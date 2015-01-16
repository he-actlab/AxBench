#!/usr/bin/python

import sys
import math


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def printUsage():
	print "Usage: python qos.py <original file> <nn file>"
	exit(1)
pass;


if(len(sys.argv) != 3):
	printUsage()

origFilename 	= sys.argv[1]
nnFilename		= sys.argv[2]

origLines 		= open(origFilename).readlines()
nnLines			= open(nnFilename).readlines()


e = 0.0
absError = 0.0

for i in range(len(origLines)):

	origLine 	= origLines[i].rstrip()
	nnLine 		= nnLines[i].rstrip()

	origPrice 	= float(origLine)
	nnPrice 	= float(nnLine)

 	nominator   = abs(origPrice - nnPrice)
 	denominator = abs(origPrice)

 	if(denominator == 0):
 		e = 1.0
 	elif(math.isnan(nominator) or (math.isnan(denominator))):
 		e = 1.0
 	elif ((nominator / denominator > 1)):
 		e = 1.0
 	else:
 		e = nominator / denominator

 	absError += e
pass;

print bcolors.FAIL	+ "*** Error: %1.8f" % (absError/float(len(origLines))) + bcolors.ENDC
