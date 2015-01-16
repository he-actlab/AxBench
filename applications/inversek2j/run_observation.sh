#!/bin/bash

rm -rf train.data/output/bin
mkdir train.data/output/bin
benchmark=inversek2j

for f in train.data/input/*.data
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	./bin/inversek2j.out ${f} train.data/output/${filename}_${benchmark}_out.data
	mv hadianeh.parroto.data train.data/output/bin/${filename}_${benchmark}.bin 
done