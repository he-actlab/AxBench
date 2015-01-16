'''
Created on Jul 22, 2012

@author: hadianeh
'''

import re
import os

class Code(object):
    PRAGMA_PARROT_KEYWORD = 'parrot'
    PRAGMA_PARROT_INPUT_KEYWORD = 'input'
    PRAGMA_PARROT_OUTPUT_KEYWORD = 'output'

    def __init__(self):
        self.src = ''
        self.regions = []
        self.name = ''
        self.type = ''
        self.tempFiles = []
    pass

    # #pragma parrot(input/output, "ParrotName",
    # ([expression])?(<expression, expression>)? expression 
    # (, ([expression])?(<expression, expression>)? expression)* )
    def parseParrotPragma(self, line, keyword):
        parrotName = None
        parrotArgs = None
        
        m = re.match(
            '#pragma\s+' +
            self.PRAGMA_PARROT_KEYWORD +
            '\s*\(\s*' +
            keyword +
            '\s*,\s*' +
            '"(.+)"' +
            '\s*,\s*' +
            '(.+)\)',
            
            line
        )
        try:
            parrotName = m.group(1)
            parrotArgs = self.parseParrotArgs(m.group(2))
            print parrotArgs

            return [True, parrotName, parrotArgs]
        except:
            return [False, parrotName, parrotArgs]
        pass
    pass

    def parseParrotArgs(self, args):
        args = re.sub(',\s+', ',', args)
        args = re.sub('\s+$', '', args)
        args = args.split(',')

        parrotArgs = []
        for a in args:
            print '-'*32
            print a
            m = re.match('^\s*(\[(.+)\])?\s*(<(.+);(.+)>)?\s*(.+)$', a)

            features = [m.group(6), m.group(2), m.group(4), m.group(5)]
            print features
            for i in range(len(features)):
                if ((features[i] == None) and (i != 1)):
                    features[i] = '0'
                pass
            
                if (features[i] == None):
                    continue
            
                features[i] = re.sub('^\s+', '', features[i])
                features[i] = re.sub('\s+$', '', features[i])
            pass

            parrotArgs.append((features[0], features[1], (features[2], features[3])))
            print parrotArgs[-1]
        pass
    
        return parrotArgs
    pass

    def buildLine(self, src, i):
        line = src[i]
        m = re.match(
            '#pragma\s+' +
            self.PRAGMA_PARROT_KEYWORD +
            '.+(\\\\)\s*$',
            line
        )

        j = i + 1
        while(m != None):
            if (j < len(src)):
                line = line[:m.start(1)] + src[j] + line[m.end(1):]
            pass
        
            m = re.match(
                '.*(\\\\)\s*$',
                line
            )
            
            j += 1
        pass
    
        return line, j - 1
    pass

    def cppParser(self, srcFileName, extCmd, outFileName):
        #self.tempFiles.append(outFileName + '.' + self.type)
        #os.system(extCmd + ' -E ' + srcFileName + ' > ' + self.tempFiles[-1])
        
        srcFile = open(self.tempFiles[-1])
        
        src = srcFile.readlines()
        self.src = src
        
        foundParrotInput = False
        self.regions = []
        inputLoc = 0
        inputParrotInfo = ()
        
        for i in range(len(src)):
            line, j = self.buildLine(src, i)
            
            if (not foundParrotInput):
                inputParrotInfo = self.parseParrotPragma(line, self.PRAGMA_PARROT_INPUT_KEYWORD)
                if (inputParrotInfo[0]):
                    foundParrotInput = True
                    inputLoc = (i, j) 
                pass
            else:
                outputParrotInfo = self.parseParrotPragma(line, self.PRAGMA_PARROT_OUTPUT_KEYWORD)
                if (outputParrotInfo[0]):
                    if (inputParrotInfo[1] != outputParrotInfo[1]):
                        errMsg = 'Error: Oops! The Parrot names do not match on line '
                        errMsg += str(inputLoc + 1) + ' "' + inputParrotInfo[1] + '"' 
                        errMsg += ' and line ' + str(i + 1) + ' "' + outputParrotInfo[1] + '"!' 
                        
                        print errMsg    
                        return False
                    pass
                    inputParrotInfo[0] = inputLoc
                    outputParrotInfo[0] = (i, j)
                    foundParrotInput = False
                    self.regions.append((inputParrotInfo, outputParrotInfo))
                pass
            pass
        pass
    
        return True
    pass

    def cppProbes(self, cfg):
        src = self.src
        
        for j, region in enumerate(self.regions):
            for i in range(2):
                loc = region[i][0]
                tag = region[i][1]
                varList = region[i][2]
             
                probeStr = ''
                for var in varList:
                    probeStr += 'parroto.write("' + tag + '", '
                    probeStr += str(i) + ', '
                    probeStr += var[0]
                    if (var[1] != None): probeStr += ', ' + var[1]
                    probeStr += ', ' + var[2][0] + ', ' + var[2][1] 
                    probeStr += ');\n'
                pass
            
                src.insert(loc[i ^ 1] + 1 + j * 2, probeStr)
            pass
        pass
    
        probeStr = '#include "' + cfg['parrotoLib'] + '"\n'
        probeStr += 'extern ' + cfg['parrotoClass'] + ' ' + cfg['parrotoObj'] + ';\n\n'
        src.insert(0, probeStr)
    
        return src
    pass
                    
    def cppCompiler(self, extCmd, outFileName):
        cmd = extCmd + ' ' + self.tempFiles[-1] + ' -o ' + outFileName
        print cmd
        os.system(cmd)
    pass 

    parsers = {
        'c':   cppParser,
        'cpp': cppParser,
        'C':   cppParser
    }
    
    probes = {
        'c':   cppProbes,
        'cpp': cppProbes,
        'C':   cppProbes
    }

    compilers = {
        'c':   cppCompiler,
        'cpp': cppCompiler,
        'C':   cppCompiler
    }
    
    def parse(self, srcFileName, extCmd, outFileName):
        self.__init__()
        
        m = re.match('(.+)\.(.+)$', srcFileName)
        
        self.name = m.group(1)
        self.type = m.group(2)
        
        self.tempFiles.append(srcFileName)
        
        self.parsers[self.type](self, srcFileName, extCmd, outFileName)
    pass

    def insertProbes(self, cfg):
        return self.probes[self.type](self, cfg)
    pass

    def compile(self, extCmd, outFileName):
        return self.compilers[self.type](self, extCmd, outFileName)
    pass 

pass

if __name__ == '__main__':
    codeRegions = Code()
    codeRegions.find('kooft.hot.cpp')
    
    exit(0)
pass
