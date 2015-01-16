#!/usr/bin/python

'''
Created on Jul 24, 2012

@author: hadianeh
'''

import argparse

from ParrotC import ParrotC

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Parrot compiler through source to source transformation. Hadi Esmaeilzadeh <hadianeh@cs.washington.edu>, July 2012'
    )
    parser.add_argument('-c', '--config', help='compilation config file')
    parser.add_argument('-a', '--action', help='compilation action: compile, observe, train, substitute')
    parser.add_argument('-e', '--extcmd', help='external compilation command', type=str)
    parser.add_argument('-s', '--source', help='source file')
    parser.add_argument('-o', '--output', help='output file')
    parser.add_argument('-t', '--track', help='track the intermediate files', action="store_true")
    
    args = parser.parse_args()
    
    if (args.action == None):
        print 'Error! Oops: Please specify the action!'
        exit(-1)
    pass

    if (args.extcmd == None):
        print 'Error! Oops: Please specify the external compilation command!'
        exit(-1)
    pass

    parrotC = ParrotC(args)
    parrotC.act(args.source, args.action.lower(), args.extcmd, args.output)

    exit(0)
pass
    