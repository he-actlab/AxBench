/*
 * ParrotEvaluator.cpp
 *
 *  Created on: Aug 8, 2012
 *      Author: hadianeh
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "FannData.h"
#include "Timer.h"
#include "CpuAffinity.h"

#include "fann.h"

using namespace std;

int n;
int nX;
int nY;
void print(float* x, float* y);

void parrot(float* x, float* y);

void runFann(struct fann *nn, float* x, int iterations);

int main(int argc, char* argv[]) {
	if (argc < 5) {
		cout << "Error: Oops! Too few parameters!\n";
		cout << "Usage: " << argv[0] << "CPU_ID PATH_TO_NN PATH_TO_DATA ACTION [N_ITERATIONS]\n";
		cout << "\t ACTION: print, parrot, fann\n";
		return -1;
	}

	struct fann *nn;
	FannData d;

	int cpuId = atoi(argv[1]);
	setCpuAffinity(cpuId);

	if (!testCpuAffinity(cpuId)) {
		cout << "Warning: Oops! The CPU affinity is not set to " << cpuId << "!\n";
	} else {
		cout << "Info: CPU affinity is set to " << cpuId << "\n";
	}

	if (!d.load(argv[3])) {
		return -1;
	}


	int iterations;
	if (argc >= 6)
		iterations = atoi(argv[5]);
	else
		iterations = 1;
	cout << "Info: Iterations: " << iterations << "\n";

	Timer t;

	n = d.getN();
	nX = d.getNX();
	nY = d.getNY();
	float* x = d.getData();

	if (strcmp(argv[4], "print") == 0) {
		cout << "print\n";
		cout << n << '\n';
		cout << nX << '\n';
		cout << nY << '\n';
		START_TIMER(t);
		d.applyFn(print);
		STOP_TIMER(t);

		cout << "time: " << t.total << "\n";
	}

	if (strcmp(argv[4], "parrot") == 0) {
		cout << "parrot\n";

		cout << "time: " << t.total << "; " << t.getTotalTime() << "; " << t.getLastTime() << "\n";

		START_TIMER(t);
		for (int j = 0; j < iterations; ++j)
			d.applyFn(parrot);
		STOP_TIMER(t);

		cout << "time: " << t.total << "; " << t.getTotalTime() << "; " << t.getLastTime() << "\n";
		cout << "\n";
	}

	if (strcmp(argv[4], "fann") == 0) {
		cout << "fann\n";

		nn = fann_create_from_file(argv[2]);

		cout << "time: " << t.total << "; " << t.getTotalTime() << "; " << t.getLastTime() << "\n";

		START_TIMER(t);
		runFann(nn, x, iterations);
		STOP_TIMER(t);

		cout << "time: " << t.total << "; " << t.getTotalTime() << "; " << t.getLastTime() << "\n";
		cout << "\n";
	}

	if (strcmp(argv[4], "verify") == 0) {
		cout << "verify\n"; cout.flush();

		fann_type* yFann;
		float* yParrot;

		yParrot = new float[nY];

		nn = fann_create_from_file(argv[2]);

		for (int i = 0; i < n; ++i) {
			cout << "--------\n";
			cout << "y*: ";
			print(x, x + nX);

			yFann = fann_run(nn, x);
			cout << "yF: ";
			print(x, yFann);

			parrot(x, yParrot);
			cout << "yP: ";
			print(x, yParrot);

			x = x + (nX + nY);
		}
	}
}

void print(float* x, float* y) {
	int i;

	cout.precision(6);
	for (i = 0; i < nX - 1; ++i)
		cout << fixed << x[i] << ',';
	cout << fixed << x[i] << ',';

	for (i = 0; i < nY - 1; ++i)
		cout << fixed << y[i] << ',';
	cout << fixed << y[i] << '\n';
}

void runFann(struct fann *nn, float* x, int iterations) {
	int i, j;
	float* xStart;
	float* y;

	xStart = x;
	for (j = 0; j < iterations; ++j) {
		x = xStart;
		for (i = 0; i < n; ++i) {
			y = fann_run(nn, x);
			x = x + (nX + nY);
		}
	}
}
