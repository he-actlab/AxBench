#include "hot.h"
#include <math.h>

#define N 2
#define M 3

class A {
public:
	int a[N * M];
};

float sine(float x[M-N][N+M]) {

#pragma parrot(input, \
		"hotFn", \
		[5+6]x[0], \
		    < 6 + 7 	; 8    >    x[1] \
		)

	x[0][0] = sin(x[0][0]);

#pragma parrot( \
	output,     \
	"hotFn",    \
	[x[1] + 10]<45; 89>x[4],       \
	x[5]        \
	)

	return x[0][0];
}


int dard(int a[N + M], A k) {
	int r;

#pragma parrot(input, "dard",\
		a[0], k.a[0])

	r = a[0] + k.a[0];

#pragma parrot  (output, "dard", <5;6>    r  )

	return r;
}

