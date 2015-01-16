#!/bin/bash

benchmark=jpeg

red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
nc='\033[0m' # No Color

for f in test.data/input/*.rgb
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	./bin/${benchmark}.nn.out 	${f} test.data/output/${filename}_nn.jpg
	./bin/${benchmark}.out 		${f} test.data/output/${filename}_orig.jpg

	compare -metric RMSE test.data/output/${filename}_nn.jpg test.data/output/${filename}_orig.jpg null > tmp.log 2> tmp.err
	echo -ne "${red}$f\t"
	awk '{ printf("*** Error: %0.2f%\n",substr($2, 2, length($2) - 2) * 100) }' tmp.err
	echo -ne "${nc}"
done