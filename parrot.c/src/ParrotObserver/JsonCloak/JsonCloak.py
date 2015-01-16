'''
Created on Aug 3, 2011

@author: hadianeh
'''

import json

class JsonCloak(object):
    def __init__(self):
        self.data = {}
    pass
    
    def put(self, field, value):
        self.data[field] = value
    pass

    def get(self, field):
        try: return self.data[field]
        except: return None
    pass
    
    def load(self, filePath):
        try: inFile = open(filePath)
        except:
            inFile = None
            print('Error! Oops! Cannot open ' + str(filePath) + '!')
            return
        pass
    
        self.data = json.loads(inFile.read())
        inFile.close()
    pass

    def save(self, filePath):
        try: outFile = open(filePath, 'w')
        except:
            outFile = None
            print('Error! Oops! Cannot open ' + filePath + '!')
            return
        pass
    
        outFile.write(str(self) + '\n')
        
        outFile.close()
    pass

    def __str__(self):
        return json.dumps(self.data, sort_keys=True, indent=2)
    pass

pass

if (__name__ == '__main__'):
    j = JsonCloak()
    
    j.set('nIn', 19)
    j.set('nOut', 1)
    j.set('nHidden', [40, 8])
    j.set('learningRate', 0.3)
    
    j.save('Kooft.json')
    
    j.load('Kooft.json')
pass

        
        
