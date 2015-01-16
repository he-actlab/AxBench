#include "fann.h"
#include "parroto.h"
extern ParrotObserver parroto;

/*
 * kinematics.cpp
 *
 *  Created on: Sep. 10 2013
 *			Author: Amir Yazdanbakhsh <yazdanbakhsh@wisc.edu>
 */

#include <cmath>
#include "kinematics.hpp"


float l1 = 0.5 ;
float l2 = 0.5 ;

void forwardk2j(float theta1, float theta2, float* x, float* y) {
	*x = l1 * cos(theta1) + l2 * cos(theta1 + theta2) ;
	*y = l1 * sin(theta1) + l2 * sin(theta1 + theta2) ;
}

void inversek2j(float x, float y, float* theta1, float* theta2) {

	double dataIn[2];
	dataIn[0] = x;
	dataIn[1] = y;

	double dataOut[2];

// #pragma parrot(input, "inversek2j", [2]dataIn)
	struct fann *ann;
	fann_type *parrotOut;
	fann_type parrotIn[2];
	parrotIn[0] = dataIn[0];
	parrotIn[1] = dataIn[1];
	ann = fann_create_from_file("fann.config/inversek2j.nn");
	parrotOut = fann_run(ann, (fann_type*)parrotIn);
// parroto.write("inversek2j", 0, dataIn, 2, 0, 0);
// 
// 	*theta2 = (float)acos(((x * x) + (y * y) - (l1 * l1) - (l2 * l2))/(2 * l1 * l2));
// 	*theta1 = (float)asin((y * (l1 + l2 * cos(*theta2)) - x * l2 * sin(*theta2))/(x * x + y * y));
// 
// 	dataOut[0] = (*theta1);
// 	dataOut[1] = (*theta2);
// 
// parroto.write("inversek2j", 1, dataOut, 2, 0.0, 2.0);
// #pragma parrot(output, "inversek2j", [2]<0.0; 2.0>dataOut)
	dataOut[0] = parrotOut[0];
	dataOut[1] = parrotOut[1];


	*theta1 = dataOut[0];
	*theta2 = dataOut[1];
}
