#!/bin/bash

BENCHMARK=sobel

red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
nc='\033[0m' # No Color

for f in test.data/input/*.rgb
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	./bin/sobel.nn.out ${f} test.data/output/${filename}_sobel.rgb
	./bin/sobel.out ${f} test.data/output/${filename}_orig.rgb

	python ../../scripts/png2rgb.py png test.data/output/${filename}_orig.rgb test.data/output/${filename}.rgb_orig.png > out1.tmp
	python ../../scripts/png2rgb.py png test.data/output/${filename}_sobel.rgb test.data/output/${filename}_sobel.png > out2.tmp
	compare -metric RMSE test.data/output/${filename}.rgb_orig.png test.data/output/${filename}_sobel.png null > tmp.log 2> tmp.err
	echo -ne "${red}$f\t"
	awk '{ printf("*** Error: %0.2f%\n",substr($2, 2, length($2) - 2) * 100) }' tmp.err
	echo -ne "${nc}"
done