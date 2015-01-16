#!/bin/bash

benchmark=fft

red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
nc='\033[0m' # No Color


./bin/${benchmark}.nn.out 	32768 test.data/output/${benchmark}_nn.data
./bin/${benchmark}.out 		32768 test.data/output/${benchmark}_orig.data

python ./scripts/qos.py test.data/output/${benchmark}_orig.data test.data/output/${benchmark}_nn.data 
