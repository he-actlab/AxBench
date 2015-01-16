#!/bin/bash

benchmark=inversek2j

red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
nc='\033[0m' # No Color

for f in test.data/input/*.data
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	./bin/${benchmark}.nn.out 	${f} test.data/output/${filename}_${benchmark}_nn.data
	./bin/${benchmark}.out 		${f} test.data/output/${filename}_${benchmark}_orig.data

	echo -ne "${red}- ${f}${nc} "

	python ./scripts/qos.py test.data/output/${filename}_${benchmark}_orig.data test.data/output/${filename}_${benchmark}_nn.data 
done