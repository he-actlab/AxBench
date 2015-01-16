#!/bin/bash
g++ -O3 -std=c++11 train_1_hidden_layers_rprop.cpp -o train_1_hidden_layers_rprop -lfann -lm
g++ -O3 -std=c++11 train_2_hidden_layers_rprop.cpp -o train_2_hidden_layers_rprop -lfann -lm
#g++ -g -lfann -std=c++11 accuracy.cpp -o accuracy
