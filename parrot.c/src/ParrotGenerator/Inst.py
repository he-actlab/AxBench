'''
Created on Jul 30, 2012

@author: hadianeh
'''

import json
from Var import Var
from Code import Code

class Inst(object):
    def __init__(self, opr='', type='s', fn = ''):
        self.opr = opr
        self.type = type.lower()
        self.fn = fn
        self.dst = Var('', [])
        self.src1 = Var('', [])
        self.src2 = Var('', [])
    pass

    def toList(self):
        l = []
        l.append(self.opr)
        l.append(self.type)
        l.append(self.fn)

        if type(self.dst) == list:
            l.append([o.toList() for o in self.dst])
        else: l.append(self.dst.toList())

        if type(self.src1) == list:
            l.append([o.toList() for o in self.src1])
        else: l.append(self.src1.toList())

        if type(self.src2) == list:
            l.append([o.toList() for o in self.src2])
        else: l.append(self.src2.toList())

        return l
    pass

    def __str__(self):
        return str(json.dumps(self.toList()))
        return str(json.dumps(self.toList(), sort_keys=True, indent=2))
    pass

    def fromList(self, l):
        self.opr = l[0]
        self.type = l[1]
        self.fn = l[2]
        self.dst.fromList(l[3])
        self.src1.fromList(l[4])
        self.src2from.List(l[5])
    pass

    def fromStr(self, s):
        self.fromList(json.loads(str(s)))
    pass

    def vectorName(self, oprList, dataType):
        name = ''
        for opr in oprList:
            name += '_' + opr.name(dataType, True)

        return name
    pass

    def cppMult(self, dataType, tabs = 1):
        s = '\t'*tabs
        if (self.type == 's'):
            s += self.dst.name(dataType)
            s += ' = '
            s += self.src1.name(dataType)
            s += ' * '
            s += self.src2.name(dataType)
            s += ';'
        pass

        if (self.type == 'v'):
            s += self.vectorName(self.dst, dataType)
            s += ' = '
            s += '_mm256_mul_ps'
            s += '('
            s += self.vectorName(self.src1, dataType)
            s += ', '
            s += self.vectorName(self.src2, dataType)
            s += ');'
        pass

        return s
    pass

    def cppAdd(self, dataType, tabs = 1):
        s = '\t'*tabs
        if (self.type == 's'):
            s += self.dst.name(dataType)
            s += ' = '
            s += self.src1.name(dataType)
            s += ' + '
            s += self.src2.name(dataType)
            s += ';'
        pass

        if (self.type == 'v'):
            s += self.vectorName(self.dst, dataType)
            s += ' = '
            s += '_mm256_add_ps'
            s += '('
            s += self.vectorName(self.src1, dataType)
            s += ', '
            s += self.vectorName(self.src2, dataType)
            s += ');'
        pass

        return s
    pass

    def cppAct(self, dataType, tabs = 1):
        s = '\t'*tabs
        if (self.type == 's'):
            s += self.dst.name(dataType)
            s += ' = '
            s += self.fn
            s += '('
            s += self.src1.name(dataType)
            s += ', '
            s += self.src2.name(dataType)
            s += ');'
        pass

        if (self.type == 'v'):
            s += self.vectorName(self.dst, dataType)
            s += ' = '
            s += self.fn
            s += '('
            s += self.vectorName(self.src1, dataType)
            s += ' /*, '
            s += self.vectorName(self.src2, dataType)
            s += ' */);'
        pass

        return s
    pass

    def cppEndn(self, dataType, tabs = 1):
        return '\t'*tabs + '// End of neuron ' + self.fn +'\n'
    pass

    def cppEndl(self, dataType, tabs = 1):
        s = '\t'*tabs + '// End of layer ' + self.fn + '\n'
        s += '\t'*tabs + '// ------------------------------\n'
        return s
    pass

    def cppAvx(self, dataType, tabs = 1):
        print self
        #raw_input()
        s = '\t'*tabs

        if (self.fn == '='):
            s += self.dst.name(dataType, False)
            s += ' = '
            s += self.src1.name(dataType, False)
            s += ';'

            return s
        pass

        if (self.fn != '_mm256_store_ps'):
            s += self.dst.name(dataType, False)
            s += ' = '
        pass
        s += self.fn
        s += '('
        if (self.fn == '_mm256_broadcast_ss'):
            s += '&' + self.src1.name(dataType, False)
        elif (self.fn == '_mm256_shuffle_ps'):
            s += self.src1.name(dataType, False)
            s += ', '
            s += self.src1.name(dataType, False)
            s += ', '
            s += self.src2.name('', False)
        elif (self.fn == '_mm256_permute2f128_ps'):
            s += self.src1.name(dataType, False)
            s += ', '
            s += self.src1.name(dataType, False)
            s += ', '
            s += self.src2.name('', False)
        elif (self.fn == '_mm256_store_ps'):
            s += '&' + self.dst.name(dataType, False)
            s += ', '
            s += self.src1.name(dataType, False)
        else:
            s += self.vectorName(self.src1, dataType)
            s += self.vectorName(self.src2, dataType)
        pass
        s += ');'

        return s
    pass

    nListCurr = []
    nListNext = []
    def convertToVector(self, code):
        pInsts = []
        postInsts = []

        if (self.opr == 'endn' or self.opr == 'endl'):
            if (self.opr == 'endl'):
                Inst.nListCurr = Inst.nListNext[:]
                Inst.nListNext = []
            pass
            return (pInsts, postInsts)
        pass

        for i, oprList in enumerate([self.dst, self.src1, self.src2]):

            oprClass = oprList[0].var
            print oprClass

            oprNames = [o.name('', True) for o in oprList]
            oprId = '_' + '_'.join(oprNames)
            print oprId, oprNames

            oprValues = [o.value for o in oprList]
            print oprValues

            oprIndices = [o.desc[-1] for o in oprList]
            print oprIndices

            if (not oprClass == ''):
                try:
                    if (code.vVars[oprId] == None):
                        continue
                except:
                    pass
                pass
            pass

            if oprClass == 'x':
                inst = Inst('avx')
                inst.fn = '_mm256_broadcast_ss'
                inst.dst = Var(oprId, [-1])
                inst.src1 = Var(oprClass, [oprIndices[0]])
                pInsts.append(inst)
                code.vVars[oprId] = None
            elif oprClass == 'w':
                code.vVars[oprId] = oprValues
            elif oprClass == 'm':
                code.vVars[oprId] = None
            elif oprClass == 'a':
                code.vVars[oprId] = None
            elif oprClass == 'n':
                if (i == 0):
                    Inst.nListNext.append((oprId, oprIndices))
                    #TODO: An ugly hack
                    code.vVars[oprId] = 'None'
                else:
                    try:
                        if (code.vVars[oprId] == None):
                            continue
                    except:
                        code.vVars[oprId] = None

                        if ((oprIndices[0] % 8) >= 4):
                            inst = Inst('avx')
                            inst.fn = '_mm256_permute2f128_ps'

                            inst.dst = Var(oprId, [-1])
                            names = [oprClass + str(j - 4) for j in oprIndices]
                            src1 = '_' + '_'.join(names)
                            if (len(oprIndices) > 4): src1 += '_tmp'
                            inst.src1 = Var(src1, [-1])
                            inst.src2 = Var('', [-1], False, '0x11')
                            pInsts.append(inst)
                        else:
                            inst = Inst('avx')
                            inst.fn = '_mm256_shuffle_ps'

                            dstVar = oprId
                            if (len(oprIndices) > 4):
                                dstVar += '_tmp'
                                code.vVars[dstVar] = None
                            pass

                            inst.dst = Var(dstVar, [-1])

                            for (srcOprId, srcOprIndices) in Inst.nListCurr:
                                if (oprIndices[0] in srcOprIndices):
                                    inst.src1 = Var(srcOprId, [-1])

                            if ((oprIndices[0] % 8) == 0 or (oprIndices[0] % 8) == 4):
                                value = '0x00'
                            if ((oprIndices[0] % 8) == 1 or (oprIndices[0] % 8) == 5):
                                value = '0x55'
                            if ((oprIndices[0] % 8) == 2 or (oprIndices[0] % 8) == 6):
                                value = '0xAA'
                            if ((oprIndices[0] % 8) == 3 or (oprIndices[0] % 8) == 7):
                                value = '0xFF'
                            inst.src2 = Var('', [-1], False, value)
                            pInsts.append(inst)

                            if (len(oprIndices) > 4):
                                inst = Inst('avx')
                                inst.fn = '_mm256_permute2f128_ps'

                                inst.dst = Var(oprId, [-1])
                                inst.src1 = Var(dstVar, [-1])

                                inst.src2 = Var('', [-1], False, '0x02')
                                pInsts.append(inst)
                            pass
                        pass
                        #x[1] = _mm256_shuffle_ps(x[0], x[0], 0xFF);
                        #x[2] = _mm256_permute2f128_ps(x[1], x[1], 0x02);
                        #x[3] = _mm256_permute2f128_ps(x[1], x[1], 0x11);
                    pass
                pass
            elif oprClass == 'y':
                code.vVars[oprId] = None

                if (len(oprIndices) == 8):
                    inst = Inst('avx')
                    inst.fn = '_mm256_store_ps'

                    inst.dst = Var(oprClass, [oprIndices[0]])
                    inst.src1 = Var(oprId, [-1])
                    inst.src2 = Var('', [-1])
                    postInsts.append(inst)
                else:
                    inst = Inst('avx')
                    inst.fn = '_mm256_store_ps'
                    inst.dst = Var(oprClass + '_tmp', [0])
                    inst.src1 = Var(oprId, [-1])
                    inst.src2 = Var('', [-1])
                    postInsts.append(inst)

                    for j in oprIndices:
                        inst = Inst('avx')
                        inst.fn = '='
                        inst.dst = Var(oprClass, [oprIndices[j]])
                        inst.src1 = Var(oprClass + '_tmp', [j])
                        inst.src2 = Var('', [-1])
                        postInsts.append(inst)
                    pass
                pass
            pass
        pass

        return (pInsts, postInsts)
    pass

    cppGens = {
        'mult': cppMult,
        'add':  cppAdd,
        'act':  cppAct,
        'endn': cppEndn,
        'endl': cppEndl,
        'avx' : cppAvx,
    }
pass

if __name__ == '__main__':
    i = Inst('add')
    i.dst = Var('x', (1, 1))
    i.src1 = Var('w', (0, 2))
    i.src2 = Var('m', (0, 2))

    print i
    print i.dst.name(), i.src1.name(), i.src2.name()

    exit(0)
pass
