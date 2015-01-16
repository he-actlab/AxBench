'''
Created on Aug 1, 2012

@author: hadianeh
'''

import json

class Var(object):
    varTrack = {}
    literalMode = True
    
    def __init__(self, name, desc, new=False, value=''):
        self.var = name
        self.desc = desc
        
        if (new):
            try: 
                self.desc.append(self.varTrack[self.var])
                self.varTrack[self.var] += 1
            except:
                self.desc.append(0)
                self.varTrack[self.var] = 1
            pass
        pass
        
        self.value = str(value)
    pass

    def reset(self):
        self.varTrack = {}
    pass
    
    def __str__(self):
        return str(json.dumps(self.toList()))
    pass

    def toList(self):
        l = list(self.desc)
        l.insert(0, self.value)
        l.insert(0, self.var)
        
        return l
    pass

    def fromList(self, l):
        try: self.var = l[0]
        except: self.var = ''
        
        try: self.value = l[1]
        except: self.var = ''
        
        try: self.desc = l[2:]
        except: self.desc = []
    pass

    def fromStr(self, s):
        self.fromDict(json.loads(str(s)))
    pass

    def name(self, dataType, vectorMode=False):
        
        literal = self.value
        if (dataType == 'float'): literal += 'f'

        if (self.var == ''):
            return literal
        
        if (len(self.value) != 0 and self.literalMode):
            if (self.var == ''):
                return literal
            else:
                if (vectorMode):
                    return self.var + str(self.desc[-1])
                else:
                    return '/* ' + self.var + '[' + str(self.desc[-1]) + ']' + ' */ ' + literal 
            pass
        pass
        
        if (self.desc[-1] < 0):
            return self.var
        
        if (vectorMode):
            return self.var + str(self.desc[-1])
        else:
            return self.var + '[' + str(self.desc[-1]) + ']'
    pass

    def isLiteral(self):
        return self.var == ''
    pass

    def convertToVector(self):
        pass
    pass
    
pass


if __name__ == '__main__':
    operand = Var('x', [0, 0])
    print operand
    
    operand = Var('x', [1, 2, 3])
    print operand

    operand = Var('w', [1, 2, 3, 4])
    print operand

    operand = Var('m', [1, 3, 4])
    print operand
    
    operand.fromList([])
    print operand

    operand.fromList(['', '', ])
    print operand
    
    operand.fromList(["w", "0.507315908911", 3, 1, 4, 37])
    print operand
pass
    