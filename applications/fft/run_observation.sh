#!/bin/bash

rm -rf train.data/output/bin
mkdir train.data/output/bin
benchmark=fft


./bin/${benchmark}.out 2048 train.data/output/${benchmark}_out.data
mv hadianeh.parroto.data train.data/output/bin/${benchmark}.bin 
