#include <math.h>
#include "floatfann.h"
#include <iostream>

using namespace std;

#define PI 3.14
#define N 400

float hotSin(float x) {
	float y;

#pragma parrot(input, "hotSin", x)

	y = sin(x);

#pragma parrot(output, "hotSin", <-1.0; 1.0>y)

	return y;
}

int main() {
	int i;
	float x, y;

	for (i = 0; i < N; ++i) {
		x = i * 2 * PI / N;
		y = hotSin(x);

		cout << x << ", " << y << "\n";
	}


}

