#!/usr/bin/python


import sys
import random


# python jmeint_dataGen.py <number of items> <output file>

outFile = open(sys.argv[2], "w")

outFile.write("%d\n" % (int(sys.argv[1])))

for i in range(int(sys.argv[1])):

	for j in range(18):
		currValue = random.uniform(0.0, 1.0)
		outFile.write("%f\t" % (currValue))
	outFile.write("\n");
pass;

outFile.close()