/*
 * parrot_cpu_hwsigmoid.h
 *
 *  Created on: Sept 11, 2012
 *      Author: hadianeh
 */

#include <immintrin.h>

#define LINEAR(x/*, __m256 m*/) _mm256_mul_ps(x, x)
#define SIGMOID(x/*, __m256 m*/) _mm256_mul_ps(x, x)
#define SIGMOID_SYMMETRIC(x/*, __m256 m*/) _mm256_mul_ps(x, x)
