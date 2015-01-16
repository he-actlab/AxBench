/*
 * ParrotVector1.h
 *
 *  Created on: Aug 3, 2012
 *      Author: hadianeh
 */

#ifndef PARROTVECTOR_H_
#define PARROTVECTOR_H_

#include <iostream>

using namespace std;

class ParrotVector {
	int xLen;
	float* x;

	int yLen;
	float* y;

public:
	ParrotVector() {
		xLen = 0;
		x = 0;

		yLen = 0;
		y = 0;
	}

	ParrotVector(int xLen, int yLen) {
		setup(xLen, yLen);
	}

	virtual ~ParrotVector() {
		if (x != 0)
			delete[] x;
		if (y != 0)
			delete[] y;
	}

	float* getX() {
		return x;
	}

	float* getY() {
		return y;
	}

	bool setup(int xLen, int yLen) {
		if (x != 0)
			delete[] x;
		if (y != 0)
			delete[] y;

		this->xLen = 0;
		this->yLen = 0;

		x = new float[xLen];
		if (x == 0)
			return false;

		y = new float[yLen];
		if (y == 0) {
			delete[] x;

			return false;
		}

		this->xLen = xLen;
		this->yLen = yLen;

		return true;
	}
};

#endif /* PARROTVECTOR_H_ */
