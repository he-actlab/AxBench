#!/bin/bash
./plang.py -c ParrotC.json -a observe -e "g++ -I../ParrotLib -c" -s "sin.cpp" -o "sin.o" -t
g++ -c -I../ParrotLib ../ParrotLib/parroto.cpp
g++ parroto.o sin.o -o sin.out
