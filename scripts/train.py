#!/bin/usr/python

# Amir Yazdanbakhsh
# Jan. 7, 2015

import os
import json
import sys
import subprocess
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
	print "python create_training.py <func name>"
	exit(1)


def main():

	if(len(sys.argv) != 2):
		printUsage()

	fName = sys.argv[1]

	# extract the layers information
	train_file = "train.data/output/fann.data/aggregated.fann"	
	f = open(train_file, "r+")
	line = f.readline()
	numIn = line.split()[1]
	numOut = line.split()[2]
	f.close()

	
	with open(fName + ".json") as data_file:
	    data = json.load(data_file)

	learning_rate 			= float(data["learning_rate"])
	epochs       		 	= int(data["epoch_number"])
	sampling_rate 			= float(data["sampling_rate"])
	test_data_fraction    	= float(data["test_data_fraction"])
	max_layer     			= int(data["max_layer_num"])
	max_neurons   			= int(data["max_neuron_num_per_layer"])

	# extract the size of training
	trainPercent = sampling_rate * test_data_fraction
	lines     = open("train.data/output/fann.data/aggregated.fann").readlines()
	totalSize = int(lines[0].rstrip().split(" ")[0])
	print >> sys.stderr, "---------------------------------------------------------",
	print >> sys.stderr, bcolors.UNDERLINE + "\n# Training Size: %d" % (totalSize * sampling_rate * (1 - test_data_fraction)) + bcolors.ENDC
	print >> sys.stderr, "---------------------------------------------------------"

	bashCommand = "bash ../../scripts/run_training.sh %s %s %s 2 %s %s %s %s %s %s" % (fName, numIn, numOut, max_neurons, max_layer, sampling_rate,test_data_fraction, learning_rate, epochs)
	print bashCommand
	process = subprocess.Popen(bashCommand.split())
	process.communicate()
pass;

if __name__ == "__main__":
    main()
