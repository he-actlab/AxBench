#!/bin/bash

rm -rf train.data/output/bin
mkdir train.data/output/bin

for f in train.data/input/*.rgb
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	./bin/sobel.out ${f} train.data/output/${filename}_sobel.rgb
	python ../../scripts/png2rgb.py png train.data/output/${filename}_sobel.rgb train.data/output/${filename}_sobel.png > /dev/null
	mv hadianeh.parroto.data train.data/output/bin/${filename}_sobel.bin 
done