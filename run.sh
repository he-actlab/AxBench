#!/bin/bash

USAGE="usage: run.sh [setup] [application name] | [make|clean|run] [blackscholes|fft|inversek2j|jmeint|jpeg|kmeans|sobel|all]"


red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
nc='\033[0m' # No Color
underline=`tput smul`
normal=`tput sgr0`

function printUsage()
{
	echo -en "\033[31m"
    echo $USAGE
    echo -en "\033[0m"
}

function printRun()
{
	echo -en "\033[36m"
    echo "running test suite ** $1 **"
    echo -en "\033[0m"
}


function MakeSrc()
{
	if [ ! -d applications/$1 ]
	then
		echo "$1 doesn't exist..."
		exit 
	fi	
	echo "enter $1, making..."
	cd applications/$1
	make &> ./log/Make.log
	if grep -q "error" ./log/Make.log; then
		grep -rn "error" ./log/Make.log
    	echo -e "${red}---------- application ** $1 ** failed during compiling (check ${applications}/log/Make.log) ----------${nc}"
    	exit 1
	fi
	echo -en "\033[36m"
	echo ""
	echo "---------- application ** $1 ** made successfully ----------"
	echo ""
	echo -en "\033[0m"
	cd -
}

function CleanSrc()
{
	if [ ! -d applications/$1 ]
	then
		echo "$1 doesn't exist..."
		exit 
	fi	
	echo "enter $1, cleaning..."
	cd applications/$1
	make clean &> ./log/clean.log
	if grep -q "error" ./log/clean.log; then
		grep -rn "error" ./log/clean.log
    	echo -e "${red}---------- application ** $1 ** failed during cleaning (check ${applications}/log/clean.log) ----------${nc}"
    	exit 1
	fi
	echo -en "\033[36m"
	echo ""
	echo "---------- application ** $1 ** has been successfully cleaned ----------"
	echo ""
	echo -en "\033[0m"
	cd -
}

function RunSrc()
{
	if [ ! -d applications/$1 ]
	then
		echo "$1 doesn't exist..."
		exit 
	fi		
	cd applications/$1
	echo -e "${green}#1: Collect the training data...${nc}"
	bash run_observation.sh
	echo -e "${green}#2: Aggregate the training data...${nc}"
	python ../../scripts/dataConv.py ./train.data/output/bin
	echo -e "${green}#3: Provide the compile parameters...${nc}"
	python ../../scripts/comm_to_json.py $1
	echo -e "${green}#4: Explore different NN topologies...${nc}"
	python ../../scripts/train.py $1 > ./log/${1}_training.log
	echo -e "${green}#5: Find the best NN topology...${nc}"
	python ../../scripts/find_best_NN.py $1
	echo -e "${green}#6: Replace the code with NN...${nc}"
	python ../../scripts/parrotConv.py $1
	echo -e "${green}#7: Compile the code with NN...${nc}"
	make -f Makefile_nn &> ./log/Make_nn.log
	if grep -q "error" ./log/Make_nn.log; then
		grep -rn "error" ./log/Make_nn.log
		echo -e "---------------------------------------------------------"
    	echo -e "${red}${underline} The transformed code for ** $1 ** failed during compiling (check ${applications}/log/Make_nn.log) ${normal}${nc}"
    	echo -e "---------------------------------------------------------"
    	exit 1
    else
    	echo -e "---------------------------------------------------------"
    	echo -e "${underline} The transformed code for ** $1 ** was successfully compiled ${normal}"
    	echo -e "---------------------------------------------------------"
	fi
	echo -e "${green}#8: Run the code on the test data...${nc}" # with Quality
	echo -e "---------------------------------------------------------"
	bash run_NN.sh
	echo -e "---------------------------------------------------------"
	echo ""
	echo -en "\033[0m"
	cd - > /dev/null
}

function SetupDir()
{
	if [ -d applications/$1 ]
		then
		echo -e "${red}Application ** $1 ** existed in the applications directory!${nc}"
		exit
	fi

	# base directory
	mkdir applications/$1
	# fann configs
	mkdir applications/$1/fann.config
	# log
	mkdir applications/$1/log
	# nn.configs
	mkdir applications/$1/nn.configs
	# src
	mkdir applications/$1/src
	# NN src
	mkdir applications/$1/src.nn
	# train data
	mkdir applications/$1/train.data
	# input and ouput
	mkdir applications/$1/train.data/input
	mkdir applications/$1/train.data/output
	mkdir applications/$1/train.data/output/fann.data
	mkdir applications/$1/train.data/output/bin
	# test data
	mkdir applications/$1/test.data
	# input and ouput
	mkdir applications/$1/test.data/input
	mkdir applications/$1/test.data/output

	echo -e "${green}Application ** $1 ** has created successfully!${nc}"	
}

#check the number of command line arguments
if [ $# -lt 2 ]
then
	printUsage
    exit
fi
if [ "$1" = "make" ]
then
	case $2 in
		"blackscholes")
			MakeSrc $2
		;;
		"fft")
			MakeSrc $2
		;;
		"inversek2j")
			MakeSrc $2
		;;
		"jmeint")
			MakeSrc $2
		;;
		"jpeg")
			MakeSrc $2
		;;
		"kmeans")
			MakeSrc $2
		;;
		"sobel")
			MakeSrc $2
		;;
		"all")
			MakeSrc blackscholes
			MakeSrc fft
			MakeSrc inversek2j
			MakeSrc	jmeint
			MakeSrc jpeg
			MakeSrc kmeans
			MakeSrc sobel
		;;
	*)
		printUsage
		exit
		;;
	esac
elif [ "$1" = "clean" ]
then
	case $2 in
		"blackscholes")
			CleanSrc $2
		;;
		"fft")
			CleanSrc $2
		;;
		"inversek2j")
			CleanSrc $2
		;;
		"jmeint")
			CleanSrc $2
		;;
		"jpeg")
			CleanSrc $2
		;;
		"kmeans")
			CleanSrc $2
		;;
		"sobel")
			CleanSrc $2
		;;
		"all")
			CleanSrc blackscholes
			CleanSrc fft
			CleanSrc inversek2j
			CleanSrc	jmeint
			CleanSrc jpeg
			CleanSrc kmeans
			CleanSrc sobel
		;;
	*)
		printUsage
		exit
		;;
	esac
elif [ "$1" = "run" ]
then
		case $2 in
		"blackscholes")
			RunSrc $2
		;;
		"fft")
			RunSrc $2
		;;
		"inversek2j")
			RunSrc $2
		;;
		"jmeint")
			RunSrc $2
		;;
		"jpeg")
			RunSrc $2
		;;
		"kmeans")
			RunSrc $2
		;;
		"sobel")
			RunSrc $2
		;;
		"all")
			RunSrc blackscholes
			RunSrc fft
			RunSrc inversek2j
			RunSrc jmeint
			RunSrc jpeg
			RunSrc kmeans
			RunSrc sobel
		;;
	*)
		printUsage
		exit
		;;
	esac
elif [ "$1" = "setup" ]
then
	SetupDir $2
else
	printUsage
	exit
fi