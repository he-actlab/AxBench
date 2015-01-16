/*
 * parrot_cpu.h
 *
 *  Created on: Aug 2, 2012
 *      Author: hadianeh
 */

#ifndef PARROT_CPU_H_
#define PARROT_CPU_H_

#include <math.h>

inline float LINEAR(float x, float m) {
    return m * x;
}

inline float SIGMOID(float x, float m) {
    float xLimit = 150. / m;
    x = x * m;

    if (x > xLimit) {
        x = xLimit;
    } else if (x < -xLimit) {
        x = -xLimit;
    }

    return 1./(1. + exp(-2. * x));
}

inline float SIGMOID_SYMMETRIC(float x, float m) {
    float xLimit = 150. / m;
    x = x * m;

    if (x > xLimit) {
        x = xLimit;
    } else if (x < -xLimit) {
        x = -xLimit;
    }

    return (2./(1. + exp(-2. * x)) - 1.0);
}

#define LINE(v1, r1, v2, r2, x)\
	(((((r2)-(r1)) * ((x)-(v1)))/((v2)-(v1))) + (r1))

#define STEPWISE(v1, v2, v3, v4, v5, v6, r1, r2, r3, r4, r5, r6, min, max, x)\
	(x < v5 ?\
		(x < v3 ?\
			(x < v2 ?\
				(x < v1 ?\
					min :\
					LINE(v1, r1, v2, r2, x)\
				) :\
				LINE(v2, r2, v3, r3, x)) :\
		 	(x < v4 ? LINE(v3, r3, v4, r4, x) :\
		LINE(v4, r4, v5, r5, x))) :\
		(x < v6 ?\
			LINE(v5, r5, v6, r6, x) :\
			max)\
	)

inline float SIGMOID_STEPWISE(float x, float m) {
    float xLimit = 150. / m;
    x = x * m;

    if (x > xLimit) {
        x = xLimit;
    } else if (x < -xLimit) {
        x = -xLimit;
    }

	return STEPWISE(
		-2.64665246009826660156e+00, -1.47221946716308593750e+00,
		-5.49306154251098632812e-01, 5.49306154251098632812e-01,
		1.47221934795379638672e+00, 2.64665293693542480469e+00,
		4.99999988824129104614e-03, 5.00000007450580596924e-02,
		2.50000000000000000000e-01, 7.50000000000000000000e-01,
		9.49999988079071044922e-01, 9.95000004768371582031e-01,
		0, 1,
		x
	);
}

inline float SIGMOID_SYMMETRIC_STEPWISE(float x, float m) {
    float xLimit = 150. / m;
    x = x * m;

    if (x > xLimit) {
        x = xLimit;
    } else if (x < -xLimit) {
        x = -xLimit;
    }

    return STEPWISE(
    	-2.64665293693542480469e+00, -1.47221934795379638672e+00,
    	-5.49306154251098632812e-01, 5.49306154251098632812e-01,
    	1.47221934795379638672e+00, 2.64665293693542480469e+00,
    	-9.90000009536743164062e-01, -8.99999976158142089844e-01,
    	-5.00000000000000000000e-01, 5.00000000000000000000e-01,
    	8.99999976158142089844e-01, 9.90000009536743164062e-01,
    	-1, 1,
    	x
    );
}

#endif /* PARROT_CPU_H_ */
