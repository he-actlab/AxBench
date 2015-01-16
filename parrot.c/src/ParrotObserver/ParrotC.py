'''
Created on Jul 20, 2012

@author: hadianeh
'''

from Code import Code
from Config import Config
import os

class ParrotC(object):
    def __init__(self, args):
        self.args = args
        
        self.cfg = Config()
        self.cfg.load(args.config)

        self.code = Code()
    pass

    def actionCompile(self, extCmd, outFileName):
        self.code.compile(extCmd, outFileName)
    pass

    def actionObserve(self, extCmd, outFileName):
        print 'Parrot Observer'
        
        cfg = self.cfg.get("Parrot Observer")
        src = self.code.insertProbes(cfg)
        
        self.code.tempFiles.append(outFileName + '.parroto.' + self.code.type)
        parrotoFile = open(self.code.tempFiles[-1], 'w')
        parrotoFile.write(''.join(src))
        parrotoFile.close()
        
        self.actionCompile(extCmd, outFileName)
        
        return True
    pass
    
    def actionTrain(self, extCmd, outFileName):
        return True
    pass

    def actionSubstitute(self, extCmd, outFileName):
        return True
    pass
    
    actions = {
        'compile':    actionCompile,
        'observe':    actionObserve,
        'train':      actionTrain,
        'substitute': actionSubstitute
    }

    def act(self, srcFileName, action, extCmd, outFileName):
        self.code.parse(srcFileName, extCmd, outFileName)
        
        try:
            self.actions[action](self, extCmd, outFileName)
            for temp in self.code.tempFiles[1:]:
                cmd = 'rm -f ' + temp
                if (self.args.track):
                    cmd = 'mv ' + temp + ' ' + temp + '.tmp'
                print cmd
                os.system(cmd)
            pass
        except:
            print('Error! Oops: Action not found! Please specify one of the following: ' 
                  + ','.join(self.actions.iterkeys()))
            return
        pass
    pass

    
pass

if __name__ == '__main__':
    parrotc = ParrotC('ParrotC.json')
    parrotc.act('kooft.hot.cpp', 'observe')
pass