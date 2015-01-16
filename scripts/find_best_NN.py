#!/bin/usr/python

# Amir Yazdanbakhsh
# Jan. 7, 2015

import os
import json
import sys
import subprocess
import shutil

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
	print "python find_best_NN.py <benchmark>"
	exit(1)

def main():
	best_NN = ""
	best_NMSE = 999999.9999
	base_path = os.getcwd()
	benchName = sys.argv[1]
	nn_path = base_path + "/nn.configs"
	for t in os.listdir(nn_path):
		# topology directory
		os.chdir(nn_path)
		epoch_dir = os.getcwd() + "/" + t
		for e in os.listdir(epoch_dir):
			if("epochs" in e):
				curr_dir=epoch_dir + "/" + e 
				os.chdir(curr_dir)
				for r in os.listdir(curr_dir):
					run_dir=curr_dir + "/" + r
					log_filename = run_dir + "/" + r + "_rprop.log"
					log = open(log_filename, "r")
					lines = log.readlines()
					index = 0
					for line in lines:
						line = line.rstrip()
						if("# Test MSE:" in line):
							line_splitt = line.split(" ")
							curr_NMSE = float(line_splitt[3])
							if(curr_NMSE < best_NMSE):
								best_NMSE = curr_NMSE
								best_NN = (e, t, r, index)
							index += 1

	# copy best NN to the folder and change its name
	(best_epoch, best_topology, best_run, best_rerun) = best_NN

	print "---------------------------------------------------------"
	print bcolors.UNDERLINE + "# Best Topology: %s" %(best_topology) + bcolors.ENDC
	print bcolors.UNDERLINE + "# Best MSE:      %1.10f" %(best_NMSE) + bcolors.ENDC
	print "---------------------------------------------------------"


	if(best_rerun == 0):
		src_file = nn_path + "/" + best_topology + "/" + best_epoch + "/" + best_run + "/" + sys.argv[1] + "_FANN_RPROP.nn"
	else:
		src_file = nn_path + "/" + best_topology + "/" + best_epoch + "/" + best_run + "/" + "rerun_" + str(best_rerun) + "/" + sys.argv[1] + "_FANN_RPROP.nn"

	dst_file = base_path + "/fann.config/" + benchName + ".nn"
	shutil.copy(src_file, dst_file)
pass;

if __name__ == "__main__":
    main()
