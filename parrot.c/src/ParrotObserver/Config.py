'''
Created on Jul 23, 2012

@author: hadianeh
'''

from JsonCloak.JsonCloak import JsonCloak

class Config(JsonCloak):
    def __init__(self):
        JsonCloak.__init__(self)
        parrotoSection = {}
        parrotoSection['parrotoLib'] = 'parroto.h'
        parrotoSection['parrotoClass'] = 'ParrotObserver'
        parrotoSection['parrotoObj'] = 'parroto'
        
        self.put('Parrot Observer', parrotoSection)
    pass

pass

if __name__ == '__main__':
    cfg = Config()
    cfg.save('Config.json')
    
    exit(0)
pass
        