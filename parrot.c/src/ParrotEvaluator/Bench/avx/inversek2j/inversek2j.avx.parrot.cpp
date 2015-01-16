/*
 * Neural Parrot!
 *
 * Hadi Esmaeilzadeh
 * <hadianeh@cs.washington.edu>
 */

#include "parrot_avx_hwsigmoid.h"

static __m256 _a0_a1 __attribute__ ((aligned(32)));
static __m256 _a0_a1_a2_a3_a4_a5_a6_a7 __attribute__ ((aligned(32)));
static __m256 _m0_m1 __attribute__ ((aligned(32)));
static __m256 _m0_m1_m2_m3_m4_m5_m6_m7 __attribute__ ((aligned(32)));
static __m256 _m10_m11 __attribute__ ((aligned(32)));
static __m256 _m12_m13 __attribute__ ((aligned(32)));
static __m256 _m14_m15 __attribute__ ((aligned(32)));
static __m256 _m2_m3 __attribute__ ((aligned(32)));
static __m256 _m4_m5 __attribute__ ((aligned(32)));
static __m256 _m6_m7 __attribute__ ((aligned(32)));
static __m256 _m8_m9 __attribute__ ((aligned(32)));
static __m256 _m8_m9_m10_m11_m12_m13_m14_m15 __attribute__ ((aligned(32)));
static __m256 _n0_n0 __attribute__ ((aligned(32)));
static __m256 _n0_n1_n2_n3_n4_n5_n6_n7 __attribute__ ((aligned(32)));
static __m256 _n1_n1 __attribute__ ((aligned(32)));
static __m256 _n2_n2 __attribute__ ((aligned(32)));
static __m256 _n3_n3 __attribute__ ((aligned(32)));
static __m256 _n4_n4 __attribute__ ((aligned(32)));
static __m256 _n5_n5 __attribute__ ((aligned(32)));
static __m256 _n6_n6 __attribute__ ((aligned(32)));
static __m256 _n7_n7 __attribute__ ((aligned(32)));
static __m256 _w0_w1_w2_w3_w4_w5_w6_w7 __attribute__ ((aligned(32))) = {0.570314403543, -5.76764185814, 0.84521121425, -1.0285536171, -3.47280740519, 1.05038296867, -2.84930765112, 3.54078720336};
static __m256 _w16_w17_w18_w19_w20_w21_w22_w23 __attribute__ ((aligned(32))) = {-0.574572193048, 4.47768072844, -0.403279381759, 0.36128697656, 0.120947451965, 0.423705678114, 0.45418197324, 1.5779174415};
static __m256 _w24_w25 __attribute__ ((aligned(32))) = {2.05729447906, -3.04649991924, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w26_w27 __attribute__ ((aligned(32))) = {-0.67993531034, 2.01102189574, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w28_w29 __attribute__ ((aligned(32))) = {6.47247669934, -10.0636974989, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w30_w31 __attribute__ ((aligned(32))) = {-4.80526802013, 8.76108241968, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w32_w33 __attribute__ ((aligned(32))) = {4.04574732792, -4.5726708561, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w34_w35 __attribute__ ((aligned(32))) = {0.0452680650865, -0.423894138827, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w36_w37 __attribute__ ((aligned(32))) = {1.66903985871, 0.642292265877, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w38_w39 __attribute__ ((aligned(32))) = {-1.50267461907, 3.65464390027, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w40_w41 __attribute__ ((aligned(32))) = {3.00110879462, -0.358736777525, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w8_w9_w10_w11_w12_w13_w14_w15 __attribute__ ((aligned(32))) = {1.32204484349, 5.44544899213, 1.00716345669, -0.36817794531, -2.32823504253, 1.16000146696, 0.633742870588, -1.93472010208};
static __m256 _x0_x0_x0_x0_x0_x0_x0_x0 __attribute__ ((aligned(32)));
static __m256 _x1_x1_x1_x1_x1_x1_x1_x1 __attribute__ ((aligned(32)));
static __m256 _y0_y1 __attribute__ ((aligned(32)));
static float y_tmp[8] __attribute__ ((aligned(32)));

/** 2 -> 8 -> 2 */
void parrot(float* x, float* y) {
	_x0_x0_x0_x0_x0_x0_x0_x0 = _mm256_broadcast_ss(&x[0]);
	_m0_m1_m2_m3_m4_m5_m6_m7 = _mm256_mul_ps(_x0_x0_x0_x0_x0_x0_x0_x0, _w0_w1_w2_w3_w4_w5_w6_w7);
	// End of neuron mult[0][0]

	_x1_x1_x1_x1_x1_x1_x1_x1 = _mm256_broadcast_ss(&x[1]);
	_m8_m9_m10_m11_m12_m13_m14_m15 = _mm256_mul_ps(_x1_x1_x1_x1_x1_x1_x1_x1, _w8_w9_w10_w11_w12_w13_w14_w15);
	// End of neuron mult[0][1]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_m0_m1_m2_m3_m4_m5_m6_m7, _m8_m9_m10_m11_m12_m13_m14_m15);
	// End of neuron add[0][1]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _w16_w17_w18_w19_w20_w21_w22_w23);
	// End of neuron add[0][2]

	_n0_n1_n2_n3_n4_n5_n6_n7 = SIGMOID_SYMMETRIC(_a0_a1_a2_a3_a4_a5_a6_a7 /*, _0.5f_0.5f_0.5f_0.5f_0.5f_0.5f_0.5f_0.5f */);
	// End of layer layer[1]
	// ------------------------------

	_n0_n0 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0x00);
	_m0_m1 = _mm256_mul_ps(_n0_n0, _w24_w25);
	// End of neuron mult[1][0]

	_n1_n1 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0x55);
	_m2_m3 = _mm256_mul_ps(_n1_n1, _w26_w27);
	// End of neuron mult[1][1]

	_n2_n2 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0xAA);
	_m4_m5 = _mm256_mul_ps(_n2_n2, _w28_w29);
	// End of neuron mult[1][2]

	_n3_n3 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0xFF);
	_m6_m7 = _mm256_mul_ps(_n3_n3, _w30_w31);
	// End of neuron mult[1][3]

	_n4_n4 = _mm256_permute2f128_ps(_n0_n0, _n0_n0, 0x11);
	_m8_m9 = _mm256_mul_ps(_n4_n4, _w32_w33);
	// End of neuron mult[1][4]

	_n5_n5 = _mm256_permute2f128_ps(_n1_n1, _n1_n1, 0x11);
	_m10_m11 = _mm256_mul_ps(_n5_n5, _w34_w35);
	// End of neuron mult[1][5]

	_n6_n6 = _mm256_permute2f128_ps(_n2_n2, _n2_n2, 0x11);
	_m12_m13 = _mm256_mul_ps(_n6_n6, _w36_w37);
	// End of neuron mult[1][6]

	_n7_n7 = _mm256_permute2f128_ps(_n3_n3, _n3_n3, 0x11);
	_m14_m15 = _mm256_mul_ps(_n7_n7, _w38_w39);
	// End of neuron mult[1][7]

	_a0_a1 = _mm256_add_ps(_m0_m1, _m2_m3);
	// End of neuron add[1][1]

	_a0_a1 = _mm256_add_ps(_a0_a1, _m4_m5);
	// End of neuron add[1][2]

	_a0_a1 = _mm256_add_ps(_a0_a1, _m6_m7);
	// End of neuron add[1][3]

	_a0_a1 = _mm256_add_ps(_a0_a1, _m8_m9);
	// End of neuron add[1][4]

	_a0_a1 = _mm256_add_ps(_a0_a1, _m10_m11);
	// End of neuron add[1][5]

	_a0_a1 = _mm256_add_ps(_a0_a1, _m12_m13);
	// End of neuron add[1][6]

	_a0_a1 = _mm256_add_ps(_a0_a1, _m14_m15);
	// End of neuron add[1][7]

	_a0_a1 = _mm256_add_ps(_a0_a1, _w40_w41);
	// End of neuron add[1][8]

	_y0_y1 = LINEAR(_a0_a1 /*, _0.5f_0.5f */);
	_mm256_store_ps(&y_tmp[0], _y0_y1);
	y[0] = y_tmp[0];
	y[1] = y_tmp[1];
	// End of layer layer[2]
	// ------------------------------

	return;
}
