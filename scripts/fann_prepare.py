#!/usr/bin/python

import sys
import random
import os
import numpy as np

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'


def printUsage():
		print "Usage: python fann_prepare.py <data_in> <sampling rate> <test data fraction>"
		print "Example: python fann_prepare.py sobel_full.data 0.01 0.3"
		exit(1)
pass;


if(len(sys.argv) != 4):
	printUsage()

# process input arguments
in_file 		= open(sys.argv[1])
samplingRate	= float(sys.argv[2])
testFraction    = float(sys.argv[3])


# read lines for processing
lines = in_file.readlines()

# some processing on the input data
basename 	= os.path.basename(sys.argv[1])
no_ext 		= os.path.splitext(basename)[0]

# create the output data
sampleFile  = open(no_ext + "_sample.fann", "w") # holds the sampled data
testFile    = open(no_ext + "_test.fann"  , "w") # holds the test data
trainFile	= open(no_ext + "_train.fann" , "w") # holds the training data

# temporary values
count 		= 1
input_data 	= []
output_data = []
data_size 	= 0
header 		= ""

# process all the lines
for line in lines:
	line = line.rstrip()
	if(count == 1):
		header = line
		count += 1
		continue;

	if(count % 2 == 0): # this is input
		input_data.append(line)
	else:
		output_data.append(line)
	count += 1

# split the header and find out the size of input
split_header = header.split(' ')
input_layer_size = len(split_header) - 1

data_size = int(split_header[0])
input_neurons = int(split_header[1])
output_neurons = int(split_header[2])



sampleSize = int(data_size * samplingRate)
trainSize  = int(sampleSize* (1 - testFraction))
testSize   = sampleSize - trainSize

# first create the sample file
sampleInputs = []
sampleOutputs = []

sampleFile.write("%d %d %d\n" % (sampleSize, input_neurons, output_neurons))
for i in range(sampleSize):
	curr_index  = random.randint(0, len(input_data) - 1)
	curr_input  = input_data[curr_index]
	curr_output = output_data[curr_index]
	sampleInputs.append(curr_input)
	sampleOutputs.append(curr_output)

	sampleFile.write("%s\n" % curr_input)
	sampleFile.write("%s\n" % curr_output)

	del input_data[curr_index]
	del output_data[curr_index]
pass;

# second create the train file
trainFile.write("%d %d %d\n" % (trainSize, input_neurons, output_neurons))
for i in range(trainSize):
	curr_index  = random.randint(0, len(sampleInputs) - 1)
	curr_input  = sampleInputs[curr_index]
	curr_output = sampleOutputs[curr_index]


	trainFile.write("%s\n" % curr_input)
	trainFile.write("%s\n" % curr_output)

	del sampleInputs[curr_index]
	del sampleOutputs[curr_index]
pass;

# finally create the test file
testFile.write("%d %d %d\n" % (testSize, input_neurons, output_neurons))
for i in range(testSize):
	curr_index  = random.randint(0, len(sampleInputs) - 1)
	curr_input  = sampleInputs[curr_index]
	curr_output = sampleOutputs[curr_index]


	testFile.write("%s\n" % curr_input)
	testFile.write("%s\n" % curr_output)

	del sampleInputs[curr_index]
	del sampleOutputs[curr_index]
pass;


sampleFile.close()
testFile.close()
trainFile.close()

