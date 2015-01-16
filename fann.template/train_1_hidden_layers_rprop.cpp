#include "fann.h"
#include <iostream>
#include <floatfann.h>

// benchmark name
// max epochs
// train file name
// # input neurons
// # hidden neurons
// # output neurons
// learning rate

int main(int argc, char *argv[])
{

	std::string benchName						= argv[1] ;
	unsigned int max_epochs      				= atoi(argv[2]) ;
	double 		 desired_error					= 0.000001 ;
	const unsigned int epochs_between_reports	= 200 ;

	std::string trainFileName 	= argv[3];
	std::string testFileName    = argv[9];


	// FANN Training
	//---------------------------------------------------------
		// Topology of the Neural Network
		const unsigned int num_layers	=	3	;

		// Number of Neurons per Layer
		const unsigned int num_input				=  atoi(argv[4]) ;
		const unsigned int num_neurons_hidden		=  atoi(argv[5]) ;
		const unsigned int num_output				=  atoi(argv[6]) ;
	//------------------------------------------------	


	// Create the Neural Network
	struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
	struct fann_train_data *train_data = fann_read_train_from_file(trainFileName.c_str());

	std::cout << "# Training: " << fann_get_training_algorithm(ann) << std::endl;


		// Set the activation function
		fann_set_activation_function_hidden(ann, FANN_SIGMOID);

		int outputActivationFunc = atoi(argv[8]);
		switch(outputActivationFunc)
		{
			case 0:
				fann_set_activation_function_output(ann, FANN_LINEAR);
				break;
			case 3:
				fann_set_activation_function_output(ann, FANN_SIGMOID);
				break;
			case 5:
				fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
				break;
			default:
				std::cerr << "\033[1;31The activation function can not be recognized.\033[0m\n";
		}

		fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);

		fann_shuffle_train_data(train_data);

		// Set learning rate
		fann_set_learning_rate(ann, atof(argv[7]));
	
		// Training
		fann_train_on_data(ann, train_data, max_epochs, epochs_between_reports, desired_error) ;
		std::string fannSave = benchName + std::string("_FANN_RPROP.nn");
		fann_save(ann, fannSave.c_str()) ;
		std::cout << "# Train MSE: " << fann_get_MSE(ann)  << std::endl;
		

		struct fann_train_data *testData = fann_read_train_from_file(testFileName.c_str());
		fann_reset_MSE(ann);
		fann_test_data(ann, testData);
		std::cout << "# Test MSE: " << fann_get_MSE(ann)   << std::endl;
		fann_destroy_train(testData);

		fann_destroy(ann) ;

	//---------------------------------------------------------
}
