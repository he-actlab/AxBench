'''
Created on Jul 31, 2012

@author: hadianeh
'''

from Var import Var

class Code(object):
    def __init__(self):
        self.insts = []
        self.vVars = {}
    pass

    def append(self, inst):
        self.insts.append(inst)
    pass

    def __str__(self):
        s = ''
        for i in self.insts:
            if i.opr == 'endn':
                s += '-'*16 + '\n'
            elif i.opr == 'endl':
                s += '#'*32 + '\n'
            else:
                s += str(i) + '\n'
            pass
        pass

        return s
    pass

    def toCpp(self, path, nn, targetConfig, vectorMode):
        try: f = open(path, 'w')
        except:
            print 'Error: Oops! Cannot open ' + path + '!'
            return False
        pass

        #TODO: parrot should be nn.name
        s  = '/*\n'
        s += ' * Neural Parrot!\n'
        s += ' *\n'
        s += ' * Hadi Esmaeilzadeh\n'
        s += ' * <hadianeh@cs.washington.edu>\n'
        s += ' */\n'
        s += '\n'

        s += '#include "' + targetConfig.get('parrotCpuLib') + '"\n'
        s += '\n'

        dataType = targetConfig.get('dataType')

        if (vectorMode):
            vectorDataType = targetConfig.get('vectorDataType')
            vVars = list(self.vVars.iterkeys())
            vVars.sort()
            for v in vVars:
                s += 'static ' + vectorDataType + ' ' + v
                s += ' __attribute__ ((aligned(32)))'
                #TODO: Ugly hack refer to the Inst.py for the matching ugly hack
                if ((self.vVars[v] != None) and (self.vVars[v] != 'None')):
                    s += ' = {'
                    i = 0
                    s += self.vVars[v][i]
                    i += 1
                    while i < len(self.vVars[v]):
                        s += ', ' + self.vVars[v][i]
                        i += 1
                    pass
                    while i < targetConfig.get('vectorWidth'):
                        s += ', ' + str(0.)
                        i += 1
                    pass
                    s += '}'
                pass
                s += ';\n'
            s += 'static ' + dataType + ' ' + 'y_tmp' + '[' + str(targetConfig.get('vectorWidth')) + '] __attribute__ ((aligned(32)));\n'
            s += '\n'
        else:
            for v, n in Var.varTrack.iteritems():
                s += 'static ' + dataType + ' ' + v + '[' + str(n) + '];\n'
            s += '\n'
        pass

        s += '/** ' + ' -> '.join([str(l) for l in nn.nNeurons]) + ' */\n'
        s += 'void ' + 'parrot' + '(' + dataType + '* x, ' + dataType + '* y) {\n'

        for i in self.insts:
            s += i.cppGens[i.opr](i, targetConfig.get('dataType')) + '\n'
        pass

        s += '\treturn;\n'
        s += '}\n'

        f.write(s)

        f.close()
        return True
    pass


pass

