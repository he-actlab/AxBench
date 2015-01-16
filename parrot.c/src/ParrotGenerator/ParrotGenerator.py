'''
Created on Jul 30, 2012

@author: hadianeh
'''

import argparse

from NeuralParrot.NeuralNet import NeuralNet
from JsonCloak.JsonCloak import JsonCloak
from Code import Code
from Inst import Inst
from Var import Var

class ParrotGenerator(object):
    def __init__(self, input, output, target, targetConfig):
        self.nn = NeuralNet()
        self.nn.load(input)

        self.output = output
        self.target = target.upper()

        self.targetConfig = JsonCloak()
        self.targetConfig.load(targetConfig)

        #TODO: This should be the name of the
        # code region which is in the neural net json file
        self.code = Code()
        self.gens[self.target](self)
        self.code.toCpp(output, self.nn, self.targetConfig, self.target == 'AVX')
    pass

    def cpuCodeGen(self):
        print self.targetConfig.get('vectorizationMode')

        w = self.targetConfig.get('vectorWidth')
        print 'vectorWidth', w
        print '~'*64

        nNeurons = self.nn.nNeurons
        print nNeurons
        nMult = 0
        nAdd = 0
        nAct = 0
        for l in range(1, len(nNeurons)):

            try:
                nMult = max([Var.varTrack['m'], nMult])
                Var.varTrack['m'] = 0
            except: nMult = 0

            try:
                nAdd = max([Var.varTrack['a'], nAdd])
                Var.varTrack['a'] = 0
            except: nAdd = 0

            try:
                nAct = max([Var.varTrack['n'], nAct])
                Var.varTrack['n'] = 0
            except: nAct = 0

            #Mult
            for i in range(nNeurons[l - 1]):
                for j in range(nNeurons[l]):
                    inst = Inst('mult')

                    inst.dst  = Var('m', [l, j, i], True)
                    if (l == 1):
                        inst.src1 = Var('x', [l - 1, i])
                    else:
                        inst.src1 = Var('n', [l - 1, i])
                    inst.src2 = Var('w', [l, j, i], True, self.nn.neurons[l - 1][j].w[i])

                    self.code.append(inst)
                pass
                endN = Inst('endn')
                endN.fn = 'mult[' + str(l - 1) + '][' + str(i) + ']'
                self.code.append(endN)
            pass

            #Add
            for i in range(1, nNeurons[l - 1] + 1):
                for j in range(nNeurons[l]):
                    inst = Inst('add')

                    if (i == 1):
                        inst.dst  = Var('a', [l, j], True)
                    else:
                        inst.dst  = Var('a', [l, j, j])
                    pass

                    if (i == nNeurons[l - 1]):
                        if (i == 1):
                            inst.src1 = Var('m', [l, j, 0, j + 0 * nNeurons[l]])
                        else:
                            inst.src1  = Var('a', [l, j, j])
                    elif (i == 1):
                        inst.src1 = Var('m', [l, j, 0, j + 0 * nNeurons[l]])
                    else:
                        inst.src1 = Var('a', [l, j, j])
                    pass

                    if (i == nNeurons[l - 1]):
                        inst.src2 = Var('w', [l, j, i], True, self.nn.neurons[l - 1][j].w[i])
                    else:
                        inst.src2 = Var('m', [l, j, i, j + i * nNeurons[l]])
                    pass

                    self.code.append(inst)
                pass
                endN = Inst('endn')
                endN.fn = 'add[' + str(l - 1) + '][' + str(i) + ']'
                self.code.append(endN)
            pass

            #Act
            for j in range(nNeurons[l]):
                inst = Inst('act')
                inst.fn = self.nn.neurons[l - 1][j].activationFn

                if (l == len(nNeurons) - 1):
                    inst.dst = Var('y', [j])
                else:
                    inst.dst = Var('n', [l, j], True)
                pass

                inst.src1 = Var('a', [l, j, j])

                inst.src2 = Var('', [l, j, -1], False, value=self.nn.neurons[l - 1][j].activationMeta)

                self.code.append(inst)
            pass

            endL = Inst('endl')
            endL.fn = 'layer[' + str(l) + ']'
            self.code.append(endL)
        pass

        Var.varTrack['m'] = nMult
        Var.varTrack['a'] = nAdd
        Var.varTrack['n'] = nAct

        print(str(self.code))
    pass


    def avxCodeGen(self):
        self.cpuCodeGen()
        self.gens[self.targetConfig.get('vectorizationMode')](self)
    pass

    def avxHorizontolCodeGen(self):
        print self.targetConfig.get('vectorizationMode')

        insts = self.code.insts

        i = 0
        vInsts = []
        while i < len(insts):
            vInstruction = Inst(insts[i].opr, 'v', insts[i].fn)
            vInstruction.dst = [insts[i].dst, ]
            vInstruction.src1 = [insts[i].src1, ]
            vInstruction.src2 = [insts[i].src2, ]
            i += 1

            j = 1
            while j < self.targetConfig.get('vectorWidth'):
                if (i >= len(insts)):
                    break

                if insts[i].opr != vInstruction.opr:
                    break

                vInstruction.dst.append(insts[i].dst)
                vInstruction.src1.append(insts[i].src1)
                vInstruction.src2.append(insts[i].src2)
                i += 1

                j += 1
            pass

            (prerequisiteInsts, postInsts) = vInstruction.convertToVector(self.code)
            vInsts.extend(prerequisiteInsts)
            vInsts.append(vInstruction)
            vInsts.extend(postInsts)
        pass

#        print '\n'*4, '#'*64
#        for i in vInsts:
#            print i
#        pass

        self.code.insts = vInsts
    pass

    def avxVerticalCodeGen(self):
        print self.targetConfig.get('vectorizationMode')
    pass

    def npuCodeGen(self):
        pass
    pass

    gens = {
        'CPU':            cpuCodeGen,
        'AVX':            avxCodeGen,
        'AVX_Horizontal': avxHorizontolCodeGen,
        'AVX_Vertical':   avxVerticalCodeGen,
        'NPU':            npuCodeGen,
    }
pass

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Parrot code generator for different targets. Hadi Esmaeilzadeh <hadianeh@cs.washington.edu>, July 2012'
    )

    parser.add_argument('-i', '--input', help='input nn file')
    parser.add_argument('-o', '--output', help='output source file')
    parser.add_argument('-t', '--target', help='target: CPU, AVX, NPU (default: CPU)', default='CPU')
    parser.add_argument('-c', '--config', help='target config file')

    args = parser.parse_args()

    if (args.input == None):
        print('Error: Oops! Please specify the input file!')
        parser.print_usage()

        exit(-1)
    pass

    if (args.output == None):
        print('Error: Oops! Please specify the output file!')
        parser.print_usage()

        exit(-1)
    pass

    if (args.config == None):
        print('Error: Oops! Please specify the target config file!')
        parser.print_usage()

        exit(-1)
    pass

    pGen = ParrotGenerator(args.input, args.output, args.target, args.config)

    exit(0)
pass





