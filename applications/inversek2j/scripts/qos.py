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


	origItem1 	= float(origLine.split("\t")[0])
	origItem2 	= float(origLine.split("\t")[1])

	nnItem1 	= float(nnLine.split("\t")[0])
 	nnItem2 	= float(nnLine.split("\t")[1])

 	diff1		= origItem1 - nnItem1
 	diff2  		= origItem2 - nnItem2

 	nominator   = math.sqrt(diff1*diff1 + diff2*diff2)
 	denominator = math.sqrt(origItem1*origItem1 + origItem2*origItem2)

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

print bcolors.FAIL	+ "*** Error: %1.8f" % (absError/len(origLines)) + bcolors.ENDC
