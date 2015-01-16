/*
 * FannData.cpp
 *
 *  Created on: Aug 3, 2012
 *      Author: hadianeh
 */

#include "FannData.h"

//#define FANNDATA_TEST
#ifdef FANNDATA_TEST

#include <iostream>

int nX;
int nY;

void print(float* x, float* y) {
	int i;

	cout.precision(6);
	for (i = 0; i < nX - 1; ++i)
		cout << fixed << x[i] << ' ';
	cout << fixed << x[i] << '\n';

	for (i = 0; i < nY - 1; ++i)
		cout << fixed << y[i] << ' ';
	cout << fixed << y[i] << '\n';
}


int main() {
	FannData d;

	d.load((char*)"fft.data");

	//d.dump(cout);

	nX = d.getNX();
	nY = d.getNY();
	cout << d.getN() << '\n';
	cout << d.getNX() << '\n';
	cout << d.getNY() << '\n';
	d.applyFn(print);
}
#endif //FANNDATA_TEST
