#!/bin/bash


# colors
red='\e[0;31m'
black='\e[0;30m'
green='\e[0;32m'
blue='\e[0;34m'
NC='\e[0m'

configs_1=()
configs_2_1=()
configs_2_2=()
MAX_RUNS=15
MAX_RERUN=10


if [ $# -ne 10 ];
    then echo "Usage: ./run_training.sh [benchmark name] [# input neurons] [# output neurons] [min hidden neurons] [max hidden neurons] [number of hidden layers] [sampling rate] [test fraction] [learning rate] [number of epochs]"
    exit
fi

# Parameter assignments
bench_name=$1
input_neurons=$2
output_neurons=$3
min_hidden_neurons=$4
max_hidden_neurons=$5
hidden_layers=$6
samplingRate=$7
testFraction=$8
learning_rate=$9
epochs=${10}




echo "Application name: 				${bench_name}"
echo "Number of input neurons: 			${input_neurons}"
echo "Number of output neurons: 		${output_neurons}"
echo "Min number of hidden neurons:		${min_hidden_neurons}"
echo "Max number of hidden neurons:		${max_hidden_neurons}"
echo "Number of hidden layers: 			${hidden_layers}"
echo "Sampling Rate: 					${samplingRate}"
echo "Test Fraction: 					${testFraction}"
echo "Learning rate: 					${learning_rate}"
echo "Number of epochs:					${epochs}"


echo "--------------- Training ${bench_name} ---------------"
base_dir=$(pwd)

# find the proper activation function for the output
outputAcctivation=`awk '{print  $1}' ./fann.config/activationFunc.cfg`


# create the directories for config files
rm -rf nn.configs
mkdir nn.configs
cd nn.configs

# one hidden layer
i=${min_hidden_neurons}
dir_before=$(pwd)
while [ $i -le ${max_hidden_neurons} ]
do
	# create the current configuration directory
	echo "# preparing the configuration ${input_neurons}_${i}_${output_neurons}..."
	mkdir ${input_neurons}_${i}_${output_neurons}
	configs_1+=(${i})
	cd ${input_neurons}_${i}_${output_neurons}
	mkdir log
	# creating all the symbolic link to the training algorithms
	ln -s ../../../../scripts/fann_prepare.py .
	ln -s ../../train.data/output/fann.data/aggregated.fann .

	mkdir epochs_${epochs}
	for ((j=1; j<=${MAX_RUNS}; j++));
	do
		mkdir epochs_${epochs}/run_${j}
		cd epochs_${epochs}/run_${j}
		ln -s ../../../../../../fann.template/train_1_hidden_layers_rprop .
		cd ../..
	done
	i=$((i*2))
	cd ${dir_before}
done
#---------------------------------------------------------------------------------

# two hidden layers
if [ ${hidden_layers} == "4" ] 
then 
	i=${min_hidden_neurons}
	k=${min_hidden_neurons}
	dir_before=$(pwd)
	while [ $i -le ${max_hidden_neurons} ]
	do
		k=${min_hidden_neurons}
		while [ $k -le ${i} ]
		do
			# create the current configuration directory
			echo "# preparing the configuration ${input_neurons}_${i}_${k}_${output_neurons}..."
			mkdir ${input_neurons}_${i}_${k}_${output_neurons}
			configs_2_1+=(${i})
			configs_2_2+=(${k})
			cd ${input_neurons}_${i}_${k}_${output_neurons}
			mkdir log
			# creating all the symbolic link to the training algorithms
			ln -s ../../../../scripts/fann_prepare.py .
			ln -s ../../train.data/output/fann.data/aggregated.fann .
		
			mkdir epochs_${epochs}
			for ((j=1; j<=${MAX_RUNS}; j++));
			do
				mkdir epochs_${epochs}/run_${j}
				cd epochs_${epochs}/run_${j}
				ln -s ../../../../../../fann.template/train_2_hidden_layers_rprop .
				cd ../..
			done
			k=$((k*2))
			cd ${dir_before}
		done
		i=$((i*2))
	done
fi
#---------------------------------------------------------------------------------


#start training NN with one hidden layer
curr_dir=$(pwd)
for i in "${configs_1[@]}"
do
	:
	cd ${input_neurons}_${i}_${output_neurons}
	cd epochs_${epochs}
	echo $(pwd)
	for ((j=1; j<=${MAX_RUNS}; j++));
	do
		curr_run_dir=$(pwd)			
		cd run_${j}		       
		echo "# config: ${input_neurons} -> ${i} -> ${output_neurons}"
		echo "# epochs: ${epochs}"
		echo "# run:    ${j}"
		echo "..............................................."

		echo $(pwd)

		# perform the sampling
		python ../../fann_prepare.py  ../../aggregated.fann ${samplingRate} ${testFraction}
 
		echo "@ Start RPROP..."0
		echo "# config: ${input_neurons}->${i}->${output_neurons}" >> run_${j}_rprop.log
		echo "# epochs: ${epochs}" >> run_${j}_rprop.log
		echo "# run:    ${j}"    >> run_${j}_rprop.log
		echo "# training algorithm: RPROP"  

		./train_1_hidden_layers_rprop ${bench_name} ${epochs} aggregated_train.fann ${input_neurons} ${i} ${output_neurons} ${learning_rate} ${outputAcctivation} aggregated_test.fann >> run_${j}_rprop.log

		for ((k=1; k<=${MAX_RERUN}; k++));
		do
			mkdir rerun_${k}
			cd rerun_${k}
			ln -s ../../../../../../../fann.template/train_1_hidden_layers_rprop
			ln -s ../aggregated_train.fann
			ln -s ../aggregated_test.fann
			ln -s ../aggregated_sample.fann
			./train_1_hidden_layers_rprop ${bench_name} ${epochs} aggregated_train.fann ${input_neurons} ${i} ${output_neurons} ${learning_rate} ${outputAcctivation} aggregated_test.fann >> ../run_${j}_rprop.log
			cd ..
		done    
		echo "***********************************************"		     
		cd ${curr_run_dir}
	done
	cd ../..
done
#---------------------------------------------------------------------------------

#start training NN with two hidden layers
if [ ${hidden_layers} == "4" ] 
then 
	cd ${curr_dir}
	#start training NN with one hidden layer
	curr_dir=$(pwd) 
	for i in "${!configs_2_1[@]}"
	do
		:
		firstIndex=${configs_2_1[$i]}
		secondIndex=${configs_2_2[$i]}
		cd ${input_neurons}_${firstIndex}_${secondIndex}_${output_neurons}
		cd epochs_${epochs}
		echo $(pwd)
		for ((j=1; j<=${MAX_RUNS}; j++));
		do
			curr_run_dir=$(pwd)			
			cd run_${j}		       
			echo "# config: ${input_neurons} -> ${firstIndex} -> ${secondIndex} -> ${output_neurons}"
			echo "# epochs: ${epochs}"
			echo "# run:    ${j}"
			echo "..............................................."

			echo $(pwd)

			# perform the sampling
			python ../../fann_prepare.py  ../../aggregated.fann ${samplingRate} ${testFraction}
	 
			echo "@ Start RPROP..."0
			echo "# config: ${input_neurons}->${i}->${output_neurons}" >> run_${j}_rprop.log
			echo "# epochs: ${epochs}" >> run_${j}_rprop.log
			echo "# run:    ${j}"    >> run_${j}_rprop.log
			echo "# training algorithm: RPROP"  

			echo "./train_2_hidden_layers_rprop ${bench_name} ${epochs} aggregated_train.fann ${input_neurons} ${firstIndex} ${secondIndex} ${output_neurons} ${learning_rate} ${outputAcctivation} aggregated_test.fann >> run_${j}_rprop.log"
			./train_2_hidden_layers_rprop ${bench_name} ${epochs} aggregated_train.fann ${input_neurons} ${firstIndex} ${secondIndex} ${output_neurons} ${learning_rate} ${outputAcctivation} aggregated_test.fann >> run_${j}_rprop.log

			for ((k=1; k<=${MAX_RERUN}; k++));
			do
				mkdir rerun_${k}
				cd rerun_${k}
				ln -s ../../../../../../../fann.template/train_2_hidden_layers_rprop
				ln -s ../aggregated_train.fann
				ln -s ../aggregated_test.fann
				ln -s ../aggregated_sample.fann
				./train_2_hidden_layers_rprop ${bench_name} ${epochs} aggregated_train.fann ${input_neurons} ${firstIndex} ${secondIndex} ${output_neurons} ${learning_rate} ${outputAcctivation} aggregated_test.fann >> ../run_${j}_rprop.log
				cd ..
			done    
			echo "***********************************************"		     
			cd ${curr_run_dir}
		done
		cd ../..
	done
fi





