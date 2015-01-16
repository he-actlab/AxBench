#!/bin/bash

rm -rf train.data/output/bin
mkdir train.data/output/bin
benchmark=kmeans
for f in train.data/input/*.rgb
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	./bin/${benchmark}.out ${f} train.data/output/${filename}_${benchmark}.rgb
	python ../../scripts/png2rgb.py png train.data/output/${filename}_${benchmark}.rgb train.data/output/${filename}_${benchmark}.png > /dev/null
	mv hadianeh.parroto.data train.data/output/bin/${filename}_${benchmark}.bin 
done