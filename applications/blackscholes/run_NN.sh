#!/bin/bash

benchmark=blackscholes

for f in test.data/input/*.data
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	./bin/${benchmark}.out ${f} test.data/output/${filename}_${benchmark}_orig.data
	./bin/${benchmark}.nn.out ${f} test.data/output/${filename}_${benchmark}_nn.data
	python ./scripts/qos.py test.data/output/${filename}_${benchmark}_orig.data test.data/output/${filename}_${benchmark}_nn.data

done