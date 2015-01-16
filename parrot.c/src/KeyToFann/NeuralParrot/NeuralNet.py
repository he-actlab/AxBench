'''
Created on Mar 3, 2012

@author: hadianeh
'''

from Neuron import Neuron
import json

class NeuralNet(object):
    def __init__(self, nNeurons = []):
        self.nNeurons = nNeurons

        self.neurons = []
        for i in range(1, len(nNeurons)):
            currLayer = []
            for j in range(nNeurons[i]):
                currLayer.append(Neuron(nNeurons[i - 1]))
            self.neurons.append(currLayer)
        pass

#        self.randomize()
    pass

    def toDict(self):
        d = {}
        d['nNeurons'] = self.nNeurons;
        for i, l in enumerate(self.neurons):
            d['l' + str(i + 1)] = [n.toDict() for n in l]

        return d 
    pass

    def __str__(self):
        return str(json.dumps(self.toDict(), sort_keys=True, indent=2))
    pass

    def fromDict(self, d):
        self.nNeurons = d['nNeurons']
        
        self.neurons = []
        for i in range(len(self.nNeurons) - 1):
            currLayer = []
            for dN in d['l' + str(i + 1)]:
                n = Neuron(0)
                n.fromDict(dN)
                currLayer.append(n)
            pass
            
            self.neurons.append(currLayer)
        pass
    pass

    def fromStr(self, s):
        self.fromDict(json.loads(str(s)))
    pass

    def randomize(self, r = (-1.0, 1.0)):
        for l in self.neurons:
            for n in l:
                n.randomize(r)
    pass

    def compute(self, x):
        x = x[:]
        
        for l in self.neurons:
            o = []
            for n in l:
                o.append(n.compute(x))
            x = o
        pass

        return x
    pass

    def load(self, path):
        try: f = open(path)
        except: 
            print 'Error: Oops! Cannot open ' + path +'!'
            return False
        pass
    
        self.fromStr(f.read())
        
        f.close()

    def save(self, path):
        try: f = open(path, 'w')
        except: 
            print 'Error: Oops! Cannot open ' + path +'!'
            return False
        pass
    
        f.write(str(self.nn))
        
        f.close()
    pass


#    def backProp(self, (x, y, d), eta):
#        x = x[:]
#        x.append(1.)
#        
#        h = []
#        for n in self.hiddenLayer:
#            o = n.compute(x)
#            h.append(o)
#            
#            n.deltaIn = []
#        pass
#    
#        for n in self.outputLayer:
#            n.compute(h)
#        pass
#    
#        err = 0
#        for i, n in enumerate(self.outputLayer):
#            delta = n.calcDelta(y[i])
#            err += 0.5 * delta ** 2
#            
#            n.backProp(eta, d)
#            
#            for j, delta in enumerate(n.deltaOut):
#                self.hiddenLayer[j].deltaIn.append(delta)
#            pass
#        pass
#    
#        for n in self.hiddenLayer:
#            n.backProp(eta, d)
#        pass
#    
#        return err
#    pass
#        
#    def train(self, (x, y, d), eta, epsilon, nEpoch):
#        eta = eta / max(d) 
#        
#        for k in range(nEpoch):
#            err = 0.
#            for i in range(len(x)):
#                err += self.backProp((x[i], y[i], d[i]), eta)
#            print k, err            
#            if (err < epsilon): break
#        pass
#    pass
#    
#    def test(self, (x, y)):
#        rmse = 0.
#        for i in range(len(x)):
#            f = self.compute(x[i])
#
#            mse = 0.
#            for j in range(len(y[i])):
#                mse += (f[j] - y[i][j]) ** 2
#            n = len(y[i])
#            mse = mse / n
#            
#            rmse += mse ** 0.5
#        pass
#        n = len(y)
#        rmse = rmse / n
#        
#        return rmse
#    pass
    

    def dump(self):
        for i, n in enumerate(self.hiddenLayer):
            print i, 'h'*64
            print n.w
        pass

        for i, n in enumerate(self.outputLayer):
            print i, 'o'*64
            print n.w
        pass
        print 'o'*64
    pass

pass

if __name__ == '__main__':
    mode = 'WEIGHTED_AND'

    if (mode == 'WEIGHTED_AND'):
        nn = NeuralNet([2, 3, 1])
        eta = 0.3
        epsilon = 1E-5
        nEpoch = 10000
        
        x = [
            [-1, -1],
            [-1, +1],
            [+1, -1],
            [+1, +1],
        ]
        
        y = [
            [+0.80],
            [-0.80],
            [-0.80],
            [+0.80],
        ]

        d = [
            1./2.,
            1./2.,
            1./2.,
            1./2.,
        ]
    
#        nn.train((x, y, d), eta, epsilon, nEpoch)
        
        print '-'*64
        print nn
        print '-'*64
        
        for i in range(len(x)):
            o = nn.compute(x[i])
            print 'x[i]:', x[i]
            print 'y[i]:', y[i], 'o:', o
        pass
    
#        print 'rmse', nn.test((x, y))
    pass

    if (mode == 'AND'):
        nn = NeuralNet(2, 2, 1)
        eta = 0.3
        epsilon = 1E-7
        nEpoch = 10000
        
        x = [
            [-1, -1],
            [-1, +1],
            [+1, -1],
            [+1, +1],
        ]
        
        y = [
            [+0.80],
            [-0.80],
            [-0.80],
            [+0.80],
        ]

        d = [
            1./4.,
            1./4.,
            1./4.,
            1./4.,
        ]
    
        nn.train((x, y, d), eta, epsilon, nEpoch)
        nn.dump()
        
        for i in range(len(x)):
            o = nn.compute(x[i])
            print x[i]
            print y[i], o

        print '&'*64
        s = nn
        print s
        
        print '&'*64
        nn.fromStr(s)
        print nn
        
        nn.dump()
            
        for i in range(len(x)):
            o = nn.compute(x[i])
            print x[i]
            print y[i], o
    pass

    
    exit(0)
pass
