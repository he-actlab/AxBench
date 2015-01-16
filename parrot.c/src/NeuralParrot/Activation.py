'''
Created on Mar 3, 2012

@author: hadianeh
'''

import math

def linear(x, m):
    return m * x
pass

def dLinear(x, y, m):
    return m
pass

def sigmoid(x, m):
    x = x * m
    xLimit = 150. / m
    if(x > xLimit):
        x = xLimit;
    elif(x < -xLimit):
        x = -xLimit;

    return 1./(1. + math.exp(-2. * x))
    return math.tanh(x)#((1 - math.exp(-x))/(1 + math.exp(-x)))
pass

def dSigmoid(x, y, m):
    return (2. * m * y * (1. - y));
pass

def sigmoidSymmetric(x, m):
    x = x * m
    xLimit = 150. / m
    if(x > xLimit):
        x = xLimit;
    elif(x < -xLimit):
        x = -xLimit;

    return (2./(1. + math.exp(-2. * x)) - 1.0)
pass

def dSigmoidSymmetric(x, y, m):
    return m * (1. - (y * y))
pass

f = {
    'LINEAR':                     (linear, dLinear),
    'THRESHOLD':                  (linear, dLinear),
    'THRESHOLD_SYMMETRIC':        (linear, dLinear),
    'SIGMOID':                    (sigmoid, dSigmoid),
    'SIGMOID_STEPWISE':           (sigmoid, dSigmoid),
    'SIGMOID_SYMMETRIC':          (sigmoidSymmetric, dSigmoidSymmetric),
    'SIGMOID_SYMMETRIC_STEPWISE': (dSigmoidSymmetric, dSigmoidSymmetric),
    'GAUSSIAN':                   (linear, dLinear),
    'GAUSSIAN_SYMMETRIC':         (linear, dLinear),
    'GAUSSIAN_STEPWISE':          (linear, dLinear),
    'ELLIOT':                     (linear, dLinear),
    'ELLIOT_SYMMETRIC':           (linear, dLinear),
    'LINEAR_PIECE':               (linear, dLinear),
    'LINEAR_PIECE_SYMMETRIC':     (linear, dLinear),
    'SIN_SYMMETRIC':              (linear, dLinear),
    'COS_SYMMETRIC':              (linear, dLinear)
}

if __name__ == '__main__':
    print f['sigmoid'][0](-100, 10)
    print f['sigmoid'][1](-100, f['sigmoid'][0](-100, 10), 10)
    
    exit(0)
pass



    