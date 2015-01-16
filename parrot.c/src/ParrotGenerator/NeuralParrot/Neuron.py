'''
Created on Mar 4, 2012

@author: hadianeh
'''

from random import random
from Activation import f
import json

class Neuron(object):
    def __init__(self, n, activationFn='SIGMOID', activationMeta=0):
        self.w = [0.] * (n + 1)
        self.x = [0.] * (n + 1)
        self.y = 0.

        self.deltaOut = [0.] * (n + 1)
        self.deltaIn  = []
        
        self.activationFn = activationFn
        self.activationMeta = activationMeta
        self.f = f[activationFn]
    pass

    def toDict(self):
        return {'w': self.w, 'activationFn': self.activationFn, 'activationMeta': self.activationMeta}
    pass

    def __str__(self):
        return str(json.dumps(self.toDict(), sort_keys=True, indent=2))
    pass

    def fromDict(self, d):
        self.__init__(len(d['w']), d['activationFn'], d['activationMeta'])
        self.w = d['w']
    pass

    def fromStr(self, s):
        self.fromDict(json.loads(s))
    pass

    def randomize(self, r = (-1.0, 1.0)):
        for i in range(len(self.w)):
            self.w[i] = r[0] + random() * (r[1] - r[0])
    pass

    def compute(self, x):
        self.x = x[0:len(self.w)]
        self.x.append(1)
                
        n = 0.
        for j in range(len(self.w)):
            n += self.w[j] * self.x[j]
        o = self.f[0](n, self.activationMeta)
        
        self.y = o
        self.df = self.f[1](n, o, self.activationMeta)
        
        return o
    pass

    # d is the distribution of training data and
    # needs to be 1 unless backprop is boosted
    def backProp(self, eta, d = 1):
        delta = sum(self.deltaIn)
        delta = self.df * delta

#        print '$'*64
#        print 'deltaIn', self.deltaIn
#        print 'w', self.w
#        print 'x', self.x
#        print 'y', self.y
        
        for i in range(len(self.w)):
            self.w[i] = self.w[i] + eta * delta * (d * self.x[i])
            self.deltaOut[i] = delta * self.w[i]
        pass
    pass

    def calcDelta(self, y):
        self.deltaIn = [y - self.y]
        return self.deltaIn[0]
    pass
    
pass

if __name__ == '__main__':
    mode = 'AND'
    
    if (mode == 'AND'):
        n = Neuron(2, 'sigmoid')
        n.randomize((-1, 1))
        eta = 0.7
        
        x = [
            [-1, -1],
            [-1, +1],
            [+1, -1],
            [+1, +1],
        ]
        
        y = [
            +0.8,
            +0.8,
            +0.8,
            -0.8,
        ]
    
    epochs = 100
    for k in range(epochs):
        err = 0
    
        for i in range(len(x)):
            n.compute(x[i])
            
            delta = n.calcDelta(y[i])
            err += 0.5 * delta ** 2 
        
            n.backProp(eta, 1)
        pass
    
        print k, err
    pass
    
    
    print n
    
    for i in range(len(x)):
        print x[i], y[i], n.compute(x[i])
    
    print '^'*64
    s = str(n)
    print s
    n.__init__(0)
    n.fromStr(s)
    
    for i in range(len(x)):
        print x[i], y[i], n.compute(x[i])
    exit(0)    
pass
