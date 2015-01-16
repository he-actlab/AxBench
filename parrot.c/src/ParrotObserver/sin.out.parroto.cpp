#include "parroto.h"
extern ParrotObserver parroto;

#include <math.h>

#include <iostream>

using namespace std;

#define PI 3.14
#define N 100

float hotSin(float x);

int main() {
	int i;
	float x, y;

	for (i = 0; i < N; ++i) {
		x = i * 2 * PI / N;
		y = hotSin(x);

		cout << x << ", " << y << "\n";
	}


}

float hotSin(float x) {
	float y;

#pragma parrot(input, "hotSin", x)
parroto.write("hotSin", 0, x, 0, 0);

	y = sin(x);
parroto.write("hotSin", 1, y, -1.0, 1.0);
#pragma parrot(output, "hotSin", <-1.0; 1.0>y)

	return y;
}

