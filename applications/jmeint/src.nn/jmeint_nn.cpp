#include "fann.h"
#include "parroto.h"
extern ParrotObserver parroto;

/*
 * fft.cpp
 *
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "tritri.hpp"

#include <fstream>
#include <iostream>
#include <map>

#include <time.h>       /* time */

#include <boost/algorithm/string/regex.hpp>


int main(int argc, char* argv[])
{
	int i;
	int x;
	int n;

	#ifndef NPU_OBSERVATION
	int x_orig ;
	#endif
	std::cout.precision(8);

	std::string inputFilename = argv[1];
	std::string outputFilename = argv[2];


	// prepare the output file for writting the theta values
	std::ofstream outputFileHandler;
	outputFileHandler.open(outputFilename);
	outputFileHandler.precision(5);

	// prepare the input file for reading the theta data
	std::ifstream inputFileHandler (inputFilename, std::ifstream::in);

	// first line defins the number of enteries
	inputFileHandler >> n;

	// create the directory for storing data
	float* xyz = (float*)malloc(n * 6 * 3 * sizeof (float)) ;
	if(xyz == NULL)
	{
		std::cout << "cannot allocate memory for the triangle coordinates!" << std::endl;
		return -1 ;
	}

	i = 0;
	while (i < n)
	{
		float a[18];
		inputFileHandler >> 	a[0] 	>> 	a[1] 	>> 	a[2] 	>> a[3] 	>> a[4] 		>> a[5] >>
						a[6]	>>	a[7]	>> 	a[8]	>> a[9]		>> a[10]		>> a[11] >>
						a[12]	>> 	a[13] 	>> 	a[14] 	>> a[15] 	>> a[16]	 	>> a[17];
		for(int j = 0 ; j < 18; j++)
		{
			xyz[i * 18 + j] = a[j];
		}
		i++;
	}

	int totalCount = 0 ;

	//double output = 0.0;


	for(i = 0 ; i < (n * 6 * 3); i += 6 * 3)
	{

		double dataIn[18];
		double dataOut[2];
		dataIn[0] 	= xyz[i + 0];
		dataIn[1] 	= xyz[i + 1];
		dataIn[2] 	= xyz[i + 2];
		dataIn[3] 	= xyz[i + 3];
		dataIn[4] 	= xyz[i + 4];
		dataIn[5] 	= xyz[i + 5];
		dataIn[6] 	= xyz[i + 6];
		dataIn[7] 	= xyz[i + 7];
		dataIn[8] 	= xyz[i + 8];

		dataIn[9]  	= xyz[i + 9];
		dataIn[10] 	= xyz[i + 10];
		dataIn[11] 	= xyz[i + 11];
		dataIn[12] 	= xyz[i + 12];
		dataIn[13] 	= xyz[i + 13];
		dataIn[14] 	= xyz[i + 14];
		dataIn[15] 	= xyz[i + 15];
		dataIn[16] 	= xyz[i + 16];
		dataIn[17] 	= xyz[i + 17];


// #pragma parrot(input, "jmeint", [18]dataIn)
	struct fann *ann;
	fann_type *parrotOut;
	fann_type parrotIn[18];
	parrotIn[0] = dataIn[0];
	parrotIn[1] = dataIn[1];
	parrotIn[2] = dataIn[2];
	parrotIn[3] = dataIn[3];
	parrotIn[4] = dataIn[4];
	parrotIn[5] = dataIn[5];
	parrotIn[6] = dataIn[6];
	parrotIn[7] = dataIn[7];
	parrotIn[8] = dataIn[8];
	parrotIn[9] = dataIn[9];
	parrotIn[10] = dataIn[10];
	parrotIn[11] = dataIn[11];
	parrotIn[12] = dataIn[12];
	parrotIn[13] = dataIn[13];
	parrotIn[14] = dataIn[14];
	parrotIn[15] = dataIn[15];
	parrotIn[16] = dataIn[16];
	parrotIn[17] = dataIn[17];
	ann = fann_create_from_file("fann.config/jmeint.nn");
	parrotOut = fann_run(ann, (fann_type*)parrotIn);
// parroto.write("jmeint", 0, dataIn, 18, 0, 0);
// 
// 		x = tri_tri_intersect(
// 				xyz + i + 0 * 3, xyz + i + 1 * 3, xyz + i + 2 * 3,
// 				xyz + i + 3 * 3, xyz + i + 4 * 3, xyz + i + 5 * 3);
// 
// 		if(x == 0)
// 		{
// 			dataOut[0] = 0.2;
// 			dataOut[1] = 0.8;
// 		}
// 		else
// 		{
// 			dataOut[0] = 0.8;
// 			dataOut[1] = 0.2;
// 		}
// 
// parroto.write("jmeint", 1, dataOut, 2, 0.2, 0.8);
// #pragma parrot(output, "jmeint", [2]<0.2; 0.8>dataOut)
	dataOut[0] = parrotOut[0];
	dataOut[1] = parrotOut[1];

		if(dataOut[0] < dataOut[1])
			x = 0;
		else
			x = 1;

		outputFileHandler << x << std::endl;
	}


	outputFileHandler.close();
	inputFileHandler.close();

	free(xyz) ;
	xyz = NULL ;

	return 0 ;
}
