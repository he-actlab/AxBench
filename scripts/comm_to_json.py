#!/bin/usr/python

# Amir Yazdanbakhsh
# Jan. 7, 2015

import sys
import json
import struct
import os

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
	print "python comm_to_json.py <func name>"
	exit(1)

def main():

	#learning rate
	#number of epochs
	#sampling rate
	#test data fraction
	#max n of layers
	#max n of neurons per layer
	
	args = []
	print "------------------ Compiler Parameters ------------------"
	args.append(raw_input(bcolors.OKBLUE + "Learning rate [0.1-1.0]: " + bcolors.ENDC))
	args.append(raw_input(bcolors.OKBLUE + "Epoch number [1-10000]: "+ bcolors.ENDC))
	args.append(raw_input(bcolors.OKBLUE + "Sampling rate [0.1-1.0]: "+ bcolors.ENDC))
	args.append(raw_input(bcolors.OKBLUE + "Test data fraction [0.1-1.0]: "+ bcolors.ENDC))

	hidden_layers = raw_input(bcolors.OKBLUE + "Maximum number of layers [3|4]: "+ bcolors.ENDC)
	while((int(hidden_layers) != 3 ) and (int(hidden_layers) != 4)):
		print(bcolors.FAIL + "Number of hidden layers can only be 3 or 4!" + bcolors.ENDC)
		hidden_layers = raw_input(bcolors.OKBLUE + "Maximum number of layers [3|4]: "+ bcolors.ENDC)

	args.append(hidden_layers)
	args.append(raw_input(bcolors.OKBLUE + "Maximum number of neurons per layer [2-64]: "+ bcolors.ENDC))
	print "---------------------------------------------------------"
	
	data_file = open(sys.argv[1] +'.json', 'w')
	#quotation marks around ints? decide after running everything
	data_file.write('{\n')
	
	data_file.write('\t\"learning_rate\": \"{}\",\n\t\"epoch_number\": \"{}\",\n'.format(args[0], args[1]))
	data_file.write('\t\"sampling_rate\": \"{}\",\n\t\"test_data_fraction\": \"{}\",\n'.format(args[2], args	[3]))
	data_file.write('\t\"max_layer_num\": \"{}\",\n\t\"max_neuron_num_per_layer\": \"{}\"\n'.format(args[4]	, args[5]))
	data_file.write('}')
	
	data_file.close()
pass;

if __name__ == "__main__":
    main()
