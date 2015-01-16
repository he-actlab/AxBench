/*
 * AvxSample.cpp
 *
 *  Created on: Aug 23, 2012
 *      Author: hadianeh
 */

//#include <avxinterin.h>
#include <immintrin.h>
#include <stdio.h>

#define AVX_WIDTH 8


void printM256(__m256* x) {
	int i;
	float a[AVX_WIDTH] __attribute__ ((aligned(AVX_WIDTH * 8)));

	_mm256_store_ps(a, *x);

	for (i = 0; i < AVX_WIDTH - 1; ++i) {
		printf("%8.2f, ", a[i]);
	}
	printf("%8.2f\n", a[i]);
}

void shuffle() {
	int i, j;
	__m256 x[3];

	float a[3 * AVX_WIDTH] __attribute__ ((aligned(AVX_WIDTH * 8)));

	for (i = 0; i < 2 * AVX_WIDTH; ++i) {
		a[i] = i;
	}

	x[0] = _mm256_load_ps(a);
	x[1] = _mm256_load_ps(a + AVX_WIDTH);

	for (i = 0; i < 1; ++i) {
		// 00 -> 0
		// 55 -> 1
		// AA -> 2
		// FF -> 3
		x[1] = _mm256_shuffle_ps(x[0], x[0], 0xFF);
		x[2] = _mm256_permute2f128_ps(x[1], x[1], 0x02);
		x[3] = _mm256_permute2f128_ps(x[1], x[1], 0x11);

		printf("%s ##########################################\n", "shuffle");
		printM256(x);
		printM256(x);
		printf("-----------------------------------------------\n");
		printM256(x + 1);
		printM256(x + 2);
		printM256(x + 3);
		printf("-----------------------------------------------\n");

		// 1 -> swap the second half with the first half
		x[1] = _mm256_permute2f128_ps(x[0], x[0], 1);
		x[1] = _mm256_shuffle_ps(x[1], x[1], 0xFF);
		//x[1] = _mm256_permute2f128_ps(x[1], x[1], 0x22);

		printf("%s ##########################################\n", "shuffle");
		printM256(x);
		printM256(x);
		printf("-----------------------------------------------\n");
		printM256(x + 1);

		// ******************************************************* //
		// ******************************************************* //
		// ******************************************************* //
		x[1] = _mm256_permute2f128_ps(x[0], x[0], 0x22);
		printf("%s ##########################################\n", "permute");
		printM256(x);
		printM256(x);
		printf("-----------------------------------------------\n");
		printM256(x + 1);

		x[1] = _mm256_insertf128_ps(x[0], _mm256_extractf128_ps(x[0], 1), 0x00);
		printf("%s ##########################################\n", "insert");
		printM256(x);
		printM256(x);
		printf("-----------------------------------------------\n");
		printM256(x + 1);

	}

}


int main() {

	shuffle();

	return 0;

	int i, j;

	//TODO: It seems there is bug in the compilation or alignment
	// that we need allocate more memory!
	float a[AVX_WIDTH] __attribute__ ((aligned(AVX_WIDTH * 8)));
	float b[AVX_WIDTH] __attribute__ ((aligned(AVX_WIDTH * 8)));
	float c[AVX_WIDTH] __attribute__ ((aligned(AVX_WIDTH * 8)));
	float d            __attribute__ ((aligned(AVX_WIDTH * 8)));
	float e[AVX_WIDTH] __attribute__ ((aligned(AVX_WIDTH * 8)));

	__m256 x;
	__m256 y;
	__m256 z;
	__m256 p;


	for (i = 0; i < 100; ++i) {
		for (j = 0; j < AVX_WIDTH; ++j) {
			a[j] = i + j;
			b[j] = i + j + AVX_WIDTH;
			e[j] = j;
		}
		d = i + j + AVX_WIDTH;

		printf("###############################################\n", i);
		printf("%-4d ##########################################\n", i);

		printf("a: ");
		for (j = 0; j < AVX_WIDTH - 1; ++j)
			printf("%8.2f, ", a[j]);
		printf("%8.2f\n", a[j]);

		printf("b: ");
		for (j = 0; j < AVX_WIDTH - 1; ++j)
			printf("%8.2f, ", b[j]);
		printf("%8.2f\n", b[j]);

		printf("d: ");
		for (j = 0; j < AVX_WIDTH - 1; ++j)
			printf("%8.2f, ", d);
		printf("%8.2f\n", d);

		printf("e: ");
		for (j = 0; j < AVX_WIDTH - 1; ++j)
			printf("%8.2f, ", e[j]);
		printf("%8.2f\n", e[j]);

		printf("-----------------------------------------------\n");
		printf("s: ");
		for (j = 0; j < AVX_WIDTH - 1; ++j)
			printf("%8.2f, ", a[j] * b[j] + e[5]);
		printf("%8.2f\n", a[j] * b[j] + e[5]);
		printf("-----------------------------------------------\n");

		x = _mm256_load_ps(a);
		y = _mm256_load_ps(b);

		//p = _mm256_broadcast_ss(&d);
		p = _mm256_broadcast_ss(e + 5);

		z = _mm256_mul_ps(x, y);
		z = _mm256_add_ps(z, p);

		_mm256_store_ps(c, p);
		printf("p: ");
		for (j = 0; j < AVX_WIDTH - 1; ++j)
			printf("%8.2f, ", c[j]);
		printf("%8.2f\n", c[j]);

		_mm256_store_ps(c, z);
		printf("v: ");
		for (j = 0; j < AVX_WIDTH - 1; ++j)
			printf("%8.2f, ", c[j]);
		printf("%8.2f\n", c[j]);
	}

}

