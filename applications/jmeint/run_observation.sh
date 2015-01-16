#!/bin/bash

rm -rf train.data/output/bin
mkdir train.data/output/bin
benchmark=jmeint

for f in train.data/input/*.data
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	./bin/${benchmark}.out ${f} train.data/output/${filename}_${benchmark}_out.data
	mv hadianeh.parroto.data train.data/output/bin/${filename}_${benchmark}.bin 
done