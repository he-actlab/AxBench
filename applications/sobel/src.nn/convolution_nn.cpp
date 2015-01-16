#include "fann.h"
#include "parroto.h"
extern ParrotObserver parroto;

/*
 * convolution.cpp
 *
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "convolution.hpp"
#include <cmath>

int total = 0;
static float kx[][3] =
		{
			{ -1, -2, -1 },
			{  0,  0,  0 },
			{  1,  2,  1 }
		} ;

static float ky[][3] =
		{
			{ -1, 0, 1 },
			{ -2, 0, 2 },
			{ -1, 0, 1 }
		} ;

float convolve(float w[][3], float k[][3])
{
	float r ;
	r = 0.0 ;
	for( int j = 0 ; j < 3 ; j++ )
		for ( int i = 0 ; i < 3 ; i++ )
		{
			r += w[i][j] * k[j][i] ;
		}
	return r ;
}

float sobel(float w[][3])
{
	float sx ;
	float sy ;
	float s  ;

	double dataIn[9];

	dataIn[0] = w[0][0];
	dataIn[1] = w[0][1];
	dataIn[2] = w[0][2];
	dataIn[3] = w[1][0];
	dataIn[4] = w[1][1];
	dataIn[5] = w[1][2];
	dataIn[6] = w[2][0];
	dataIn[7] = w[2][1];
	dataIn[8] = w[2][2];

// #pragma parrot(input, "sobel", [9]dataIn)
	struct fann *ann;
	fann_type *parrotOut;
	fann_type parrotIn[9];
	parrotIn[0] = dataIn[0];
	parrotIn[1] = dataIn[1];
	parrotIn[2] = dataIn[2];
	parrotIn[3] = dataIn[3];
	parrotIn[4] = dataIn[4];
	parrotIn[5] = dataIn[5];
	parrotIn[6] = dataIn[6];
	parrotIn[7] = dataIn[7];
	parrotIn[8] = dataIn[8];
	ann = fann_create_from_file("fann.config/sobel.nn");
	parrotOut = fann_run(ann, (fann_type*)parrotIn);
// parroto.write("sobel", 0, dataIn, 9, 0, 0);
// 
// 	for(int i = 0 ; i < 3 ; i++)
// 		for(int j = 0 ; j < 3 ; j++)
// 			if(DEBUG) std::cout << "w_ " << i << j << ": " << w[i][j] << std::endl;
// 	sx = convolve(w, ky) ;
// 	if(DEBUG) std::cout << "sx: " << sx << std::endl ;
// 	sy = convolve(w, kx) ;
// 	if(DEBUG) std::cout << "sy: " << sy << std::endl ;
// 
// 	s = sqrt(sx * sx + sy * sy) ;
// 	if (s >= (256 / sqrt(256 * 256 + 256 * 256)))
// 		s = 255 / sqrt(256 * 256 + 256 * 256);
// 
// parroto.write("sobel", 1, s, 0.0, 1.0);
// #pragma parrot(output, "sobel", <0.0; 1.0>s)
	s = parrotOut[0];

	return s ;
}
