/*
 * Neural Parrot!
 *
 * Hadi Esmaeilzadeh
 * <hadianeh@cs.washington.edu>
 */

#include <immintrin.h>
#include "parrot_cpu.h"
#include "Timer.h"

#define PARROT_AVX

#define AVX_WIDTH 8

static float a[4]  __attribute__ ((aligned(AVX_WIDTH * 8)));
static float m[16] __attribute__ ((aligned(AVX_WIDTH * 8)));
static float w[38] __attribute__ ((aligned(AVX_WIDTH * 8)));
static float n[4]  __attribute__ ((aligned(AVX_WIDTH * 8)));

static __m256 vX[1];
static __m256 vA[4];
static __m256 vM[16];
static __m256 vW[38];
static __m256 vN[4];

/** 1 -> 4 -> 4 -> 2 */
void parrot_avx(float* x, float* y) {
#ifndef PARROT_AVX
	m[0] = x[0] * /* w[0] */ 3.67738320954;
	m[1] = x[0] * /* w[1] */ 4.22986682717;
	m[2] = x[0] * /* w[2] */ 47.7833731923;
	m[3] = x[0] * /* w[3] */ -0.289138853497;
#else
	vW[0] = _mm256_set_ps(-0.289138853497, 47.7833731923, 4.22986682717, 3.67738320954, -0.289138853497, 47.7833731923, 4.22986682717, 3.67738320954);
	vX[0] = _mm256_broadcast_ss(x + 0); // 4
	vM[0] = _mm256_mul_ps(vX[0], vW[0]); // 4
#endif
	// End of neuron

#ifndef PARROT_AVX
	a[0] = m[0] + /* w[4] */ -0.961986326027;
	a[1] = m[1] + /* w[5] */ -0.918933481023;
	a[2] = m[2] + /* w[6] */ -1.23577104176;
	a[3] = m[3] + /* w[7] */ 1.31340158088;
#else
	vA[0] = _mm256_add_ps(vM[0], vW[1]); // 4
#endif
	// End of neuron

#ifndef PARROT_AVX
	n[0] = a[0] * m[0]; //SIGMOID_SYMMETRIC(a[0], 0.5);
	n[1] = a[1] * m[1]; //SIGMOID_SYMMETRIC(a[1], 0.5);
	n[2] = a[2] * m[2]; //SIGMOID_SYMMETRIC(a[2], 0.5);
	n[3] = a[3] * m[3]; //SIGMOID_SYMMETRIC(a[3], 0.5);
#else
	vN[0] = _mm256_mul_ps(vA[0], vM[0]); // 4
#endif
	// End of layer

#ifndef PARROT_AVX
	m[0] = n[0] * /* w[8] */ -0.904775977622;
	m[1] = n[0] * /* w[9] */ 1.03219169414;
	m[2] = n[0] * /* w[10] */ 24.0233096975;
	m[3] = n[0] * /* w[11] */ -2.17828471916;
	// End of neuron

	m[4] = n[1] * /* w[12] */ -0.498271330942;
	m[5] = n[1] * /* w[13] */ -11.4615947491;
	m[6] = n[1] * /* w[14] */ 0.232458753702;
	m[7] = n[1] * /* w[15] */ -10.3649629992;
	// End of neuron
#else
	n0 = _mm256_shuffle_ps(vN[0], vN[0], 0x00);
#endif

	m[8] = n[2] * /* w[16] */ -0.95811968768;
	m[9] = n[2] * /* w[17] */ 3.73359122884;
	m[10] = n[2] * /* w[18] */ -0.812900428706;
	m[11] = n[2] * /* w[19] */ -1.80856321996;
	// End of neuron

	m[12] = n[3] * /* w[20] */ 0.3290029729;
	m[13] = n[3] * /* w[21] */ -0.227268037464;
	m[14] = n[3] * /* w[22] */ -1.62601703857;
	m[15] = n[3] * /* w[23] */ 0.64278959825;
	// End of neuron

	a[0] = m[0] + m[4];
	a[1] = m[1] + m[5];
	a[2] = m[2] + m[6];
	a[3] = m[3] + m[7];
	// End of neuron

	a[0] = a[0] + m[8];
	a[1] = a[1] + m[9];
	a[2] = a[2] + m[10];
	a[3] = a[3] + m[11];
	// End of neuron

	a[0] = a[0] + m[12];
	a[1] = a[1] + m[13];
	a[2] = a[2] + m[14];
	a[3] = a[3] + m[15];
	// End of neuron

	a[0] = a[0] + /* w[24] */ 0.21797340615;
	a[1] = a[1] + /* w[25] */ 1.24090729436;
	a[2] = a[2] + /* w[26] */ -0.445888097466;
	a[3] = a[3] + /* w[27] */ 0.312022772993;
	// End of neuron

	n[0] = a[0] * m[12]; //SIGMOID_SYMMETRIC(a[0], 0.5);
	n[1] = a[1] * m[13]; //SIGMOID_SYMMETRIC(a[1], 0.5);
	n[2] = a[2] * m[14]; //SIGMOID_SYMMETRIC(a[2], 0.5);
	n[3] = a[3] * m[15]; //SIGMOID_SYMMETRIC(a[3], 0.5);
	// End of layer

	m[0] = n[0] * /* w[28] */ 1.07551426483;
	m[1] = n[0] * /* w[29] */ 7.20508247617;
	// End of neuron

	m[2] = n[1] * /* w[30] */ -1.69827827052;
	m[3] = n[1] * /* w[31] */ 1.82238857165;
	// End of neuron

	m[4] = n[2] * /* w[32] */ 1.66747941318;
	m[5] = n[2] * /* w[33] */ 0.0189889050494;
	// End of neuron

	m[6] = n[3] * /* w[34] */ 2.18012104324;
	m[7] = n[3] * /* w[35] */ 0.344916990516;
	// End of neuron

	a[0] = m[0] + m[2];
	a[1] = m[1] + m[3];
	// End of neuron

	a[0] = a[0] + m[4];
	a[1] = a[1] + m[5];
	// End of neuron

	a[0] = a[0] + m[6];
	a[1] = a[1] + m[7];
	// End of neuron

	a[0] = a[0] + /* w[36] */ 0.388499855295;
	a[1] = a[1] + /* w[37] */ 0.507315908911;
	// End of neuron

	y[0] = a[0] * m[6]; //SIGMOID_SYMMETRIC(a[0], 0.5);
	y[1] = a[1] * m[7]; //SIGMOID_SYMMETRIC(a[1], 0.5);
	// End of layer

	return;
}

/** 1 -> 4 -> 4 -> 2 */
void parrot(float* x, float* y) {
	m[0] = x[0] * /* w[0] */ 3.67738320954;
	m[1] = x[0] * /* w[1] */ 4.22986682717;
	m[2] = x[0] * /* w[2] */ 47.7833731923;
	m[3] = x[0] * /* w[3] */ -0.289138853497;
	// End of neuron

	a[0] = m[0] + /* w[4] */ -0.961986326027;
	a[1] = m[1] + /* w[5] */ -0.918933481023;
	a[2] = m[2] + /* w[6] */ -1.23577104176;
	a[3] = m[3] + /* w[7] */ 1.31340158088;
	// End of neuron

	n[0] = a[0] * m[0]; //SIGMOID_SYMMETRIC(a[0], 0.5);
	n[1] = a[1] * m[1]; //SIGMOID_SYMMETRIC(a[1], 0.5);
	n[2] = a[2] * m[2]; //SIGMOID_SYMMETRIC(a[2], 0.5);
	n[3] = a[3] * m[3]; //SIGMOID_SYMMETRIC(a[3], 0.5);
	// End of layer

	m[0] = n[0] * /* w[8] */ -0.904775977622;
	m[1] = n[0] * /* w[9] */ 1.03219169414;
	m[2] = n[0] * /* w[10] */ 24.0233096975;
	m[3] = n[0] * /* w[11] */ -2.17828471916;
	// End of neuron

	m[4] = n[1] * /* w[12] */ -0.498271330942;
	m[5] = n[1] * /* w[13] */ -11.4615947491;
	m[6] = n[1] * /* w[14] */ 0.232458753702;
	m[7] = n[1] * /* w[15] */ -10.3649629992;
	// End of neuron

	m[8] = n[2] * /* w[16] */ -0.95811968768;
	m[9] = n[2] * /* w[17] */ 3.73359122884;
	m[10] = n[2] * /* w[18] */ -0.812900428706;
	m[11] = n[2] * /* w[19] */ -1.80856321996;
	// End of neuron

	m[12] = n[3] * /* w[20] */ 0.3290029729;
	m[13] = n[3] * /* w[21] */ -0.227268037464;
	m[14] = n[3] * /* w[22] */ -1.62601703857;
	m[15] = n[3] * /* w[23] */ 0.64278959825;
	// End of neuron

	a[0] = m[0] + m[4];
	a[1] = m[1] + m[5];
	a[2] = m[2] + m[6];
	a[3] = m[3] + m[7];
	// End of neuron

	a[0] = a[0] + m[8];
	a[1] = a[1] + m[9];
	a[2] = a[2] + m[10];
	a[3] = a[3] + m[11];
	// End of neuron

	a[0] = a[0] + m[12];
	a[1] = a[1] + m[13];
	a[2] = a[2] + m[14];
	a[3] = a[3] + m[15];
	// End of neuron

	a[0] = a[0] + /* w[24] */ 0.21797340615;
	a[1] = a[1] + /* w[25] */ 1.24090729436;
	a[2] = a[2] + /* w[26] */ -0.445888097466;
	a[3] = a[3] + /* w[27] */ 0.312022772993;
	// End of neuron

	n[0] = a[0] * m[12]; //SIGMOID_SYMMETRIC(a[0], 0.5);
	n[1] = a[1] * m[13]; //SIGMOID_SYMMETRIC(a[1], 0.5);
	n[2] = a[2] * m[14]; //SIGMOID_SYMMETRIC(a[2], 0.5);
	n[3] = a[3] * m[15]; //SIGMOID_SYMMETRIC(a[3], 0.5);
	// End of layer

	m[0] = n[0] * /* w[28] */ 1.07551426483;
	m[1] = n[0] * /* w[29] */ 7.20508247617;
	// End of neuron

	m[2] = n[1] * /* w[30] */ -1.69827827052;
	m[3] = n[1] * /* w[31] */ 1.82238857165;
	// End of neuron

	m[4] = n[2] * /* w[32] */ 1.66747941318;
	m[5] = n[2] * /* w[33] */ 0.0189889050494;
	// End of neuron

	m[6] = n[3] * /* w[34] */ 2.18012104324;
	m[7] = n[3] * /* w[35] */ 0.344916990516;
	// End of neuron

	a[0] = m[0] + m[2];
	a[1] = m[1] + m[3];
	// End of neuron

	a[0] = a[0] + m[4];
	a[1] = a[1] + m[5];
	// End of neuron

	a[0] = a[0] + m[6];
	a[1] = a[1] + m[7];
	// End of neuron

	a[0] = a[0] + /* w[36] */ 0.388499855295;
	a[1] = a[1] + /* w[37] */ 0.507315908911;
	// End of neuron

	y[0] = a[0] * m[6]; //SIGMOID_SYMMETRIC(a[0], 0.5);
	y[1] = a[1] * m[7]; //SIGMOID_SYMMETRIC(a[1], 0.5);
	// End of layer

	return;
}
