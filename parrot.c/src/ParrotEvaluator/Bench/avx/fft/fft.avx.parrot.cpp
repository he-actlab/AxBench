/*
 * Neural Parrot!
 *
 * Hadi Esmaeilzadeh
 * <hadianeh@cs.washington.edu>
 */

#include "parrot_avx_hwsigmoid.h"

static __m256 _a0_a1 __attribute__ ((aligned(32)));
static __m256 _a0_a1_a2_a3 __attribute__ ((aligned(32)));
static __m256 _m0_m1 __attribute__ ((aligned(32)));
static __m256 _m0_m1_m2_m3 __attribute__ ((aligned(32)));
static __m256 _m12_m13_m14_m15 __attribute__ ((aligned(32)));
static __m256 _m2_m3 __attribute__ ((aligned(32)));
static __m256 _m4_m5 __attribute__ ((aligned(32)));
static __m256 _m4_m5_m6_m7 __attribute__ ((aligned(32)));
static __m256 _m6_m7 __attribute__ ((aligned(32)));
static __m256 _m8_m9_m10_m11 __attribute__ ((aligned(32)));
static __m256 _n0_n0 __attribute__ ((aligned(32)));
static __m256 _n0_n0_n0_n0 __attribute__ ((aligned(32)));
static __m256 _n0_n1_n2_n3 __attribute__ ((aligned(32)));
static __m256 _n1_n1 __attribute__ ((aligned(32)));
static __m256 _n1_n1_n1_n1 __attribute__ ((aligned(32)));
static __m256 _n2_n2 __attribute__ ((aligned(32)));
static __m256 _n2_n2_n2_n2 __attribute__ ((aligned(32)));
static __m256 _n3_n3 __attribute__ ((aligned(32)));
static __m256 _n3_n3_n3_n3 __attribute__ ((aligned(32)));
static __m256 _w0_w1_w2_w3 __attribute__ ((aligned(32))) = {3.67738320954, 4.22986682717, 47.7833731923, -0.289138853497, 0.0, 0.0, 0.0, 0.0};
static __m256 _w12_w13_w14_w15 __attribute__ ((aligned(32))) = {-0.498271330942, -11.4615947491, 0.232458753702, -10.3649629992, 0.0, 0.0, 0.0, 0.0};
static __m256 _w16_w17_w18_w19 __attribute__ ((aligned(32))) = {-0.95811968768, 3.73359122884, -0.812900428706, -1.80856321996, 0.0, 0.0, 0.0, 0.0};
static __m256 _w20_w21_w22_w23 __attribute__ ((aligned(32))) = {0.3290029729, -0.227268037464, -1.62601703857, 0.64278959825, 0.0, 0.0, 0.0, 0.0};
static __m256 _w24_w25_w26_w27 __attribute__ ((aligned(32))) = {0.21797340615, 1.24090729436, -0.445888097466, 0.312022772993, 0.0, 0.0, 0.0, 0.0};
static __m256 _w28_w29 __attribute__ ((aligned(32))) = {1.07551426483, 7.20508247617, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w30_w31 __attribute__ ((aligned(32))) = {-1.69827827052, 1.82238857165, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w32_w33 __attribute__ ((aligned(32))) = {1.66747941318, 0.0189889050494, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w34_w35 __attribute__ ((aligned(32))) = {2.18012104324, 0.344916990516, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w36_w37 __attribute__ ((aligned(32))) = {0.388499855295, 0.507315908911, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w4_w5_w6_w7 __attribute__ ((aligned(32))) = {-0.961986326027, -0.918933481023, -1.23577104176, 1.31340158088, 0.0, 0.0, 0.0, 0.0};
static __m256 _w8_w9_w10_w11 __attribute__ ((aligned(32))) = {-0.904775977622, 1.03219169414, 24.0233096975, -2.17828471916, 0.0, 0.0, 0.0, 0.0};
static __m256 _x0_x0_x0_x0 __attribute__ ((aligned(32)));
static __m256 _y0_y1 __attribute__ ((aligned(32)));
static float y_tmp[8] __attribute__ ((aligned(32)));

/** 1 -> 4 -> 4 -> 2 */
void parrot(float* x, float* y) {
	_x0_x0_x0_x0 = _mm256_broadcast_ss(&x[0]);
	_m0_m1_m2_m3 = _mm256_mul_ps(_x0_x0_x0_x0, _w0_w1_w2_w3);
	// End of neuron mult[0][0]

	_a0_a1_a2_a3 = _mm256_add_ps(_m0_m1_m2_m3, _w4_w5_w6_w7);
	// End of neuron add[0][1]

	_n0_n1_n2_n3 = SIGMOID_SYMMETRIC(_a0_a1_a2_a3 /*, _0.5f_0.5f_0.5f_0.5f */);
	// End of layer layer[1]
	// ------------------------------

	_n0_n0_n0_n0 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0x00);
	_m0_m1_m2_m3 = _mm256_mul_ps(_n0_n0_n0_n0, _w8_w9_w10_w11);
	// End of neuron mult[1][0]

	_n1_n1_n1_n1 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0x55);
	_m4_m5_m6_m7 = _mm256_mul_ps(_n1_n1_n1_n1, _w12_w13_w14_w15);
	// End of neuron mult[1][1]

	_n2_n2_n2_n2 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0xAA);
	_m8_m9_m10_m11 = _mm256_mul_ps(_n2_n2_n2_n2, _w16_w17_w18_w19);
	// End of neuron mult[1][2]

	_n3_n3_n3_n3 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0xFF);
	_m12_m13_m14_m15 = _mm256_mul_ps(_n3_n3_n3_n3, _w20_w21_w22_w23);
	// End of neuron mult[1][3]

	_a0_a1_a2_a3 = _mm256_add_ps(_m0_m1_m2_m3, _m4_m5_m6_m7);
	// End of neuron add[1][1]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _m8_m9_m10_m11);
	// End of neuron add[1][2]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _m12_m13_m14_m15);
	// End of neuron add[1][3]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _w24_w25_w26_w27);
	// End of neuron add[1][4]

	_n0_n1_n2_n3 = SIGMOID_SYMMETRIC(_a0_a1_a2_a3 /*, _0.5f_0.5f_0.5f_0.5f */);
	// End of layer layer[2]
	// ------------------------------

	_n0_n0 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0x00);
	_m0_m1 = _mm256_mul_ps(_n0_n0, _w28_w29);
	// End of neuron mult[2][0]

	_n1_n1 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0x55);
	_m2_m3 = _mm256_mul_ps(_n1_n1, _w30_w31);
	// End of neuron mult[2][1]

	_n2_n2 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0xAA);
	_m4_m5 = _mm256_mul_ps(_n2_n2, _w32_w33);
	// End of neuron mult[2][2]

	_n3_n3 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0xFF);
	_m6_m7 = _mm256_mul_ps(_n3_n3, _w34_w35);
	// End of neuron mult[2][3]

	_a0_a1 = _mm256_add_ps(_m0_m1, _m2_m3);
	// End of neuron add[2][1]

	_a0_a1 = _mm256_add_ps(_a0_a1, _m4_m5);
	// End of neuron add[2][2]

	_a0_a1 = _mm256_add_ps(_a0_a1, _m6_m7);
	// End of neuron add[2][3]

	_a0_a1 = _mm256_add_ps(_a0_a1, _w36_w37);
	// End of neuron add[2][4]

	_y0_y1 = SIGMOID_SYMMETRIC(_a0_a1 /*, _0.5f_0.5f */);
	_mm256_store_ps(&y_tmp[0], _y0_y1);
	y[0] = y_tmp[0];
	y[1] = y_tmp[1];
	// End of layer layer[3]
	// ------------------------------

	return;
}
