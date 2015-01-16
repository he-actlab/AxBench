/*
 * Neural Parrot!
 *
 * Hadi Esmaeilzadeh
 * <hadianeh@cs.washington.edu>
 */

#include "parrot_cpu.h"

static float a[8];
static float m[16];
static float w[42];
static float n[8];

/** 2 -> 8 -> 2 */
void parrot(float* x, float* y) {
	m[0] = x[0] * /* w[0] */ 0.570314403543f;
	m[1] = x[0] * /* w[1] */ -5.76764185814f;
	m[2] = x[0] * /* w[2] */ 0.84521121425f;
	m[3] = x[0] * /* w[3] */ -1.0285536171f;
	m[4] = x[0] * /* w[4] */ -3.47280740519f;
	m[5] = x[0] * /* w[5] */ 1.05038296867f;
	m[6] = x[0] * /* w[6] */ -2.84930765112f;
	m[7] = x[0] * /* w[7] */ 3.54078720336f;
	// End of neuron

	m[8] = x[1] * /* w[8] */ 1.32204484349f;
	m[9] = x[1] * /* w[9] */ 5.44544899213f;
	m[10] = x[1] * /* w[10] */ 1.00716345669f;
	m[11] = x[1] * /* w[11] */ -0.36817794531f;
	m[12] = x[1] * /* w[12] */ -2.32823504253f;
	m[13] = x[1] * /* w[13] */ 1.16000146696f;
	m[14] = x[1] * /* w[14] */ 0.633742870588f;
	m[15] = x[1] * /* w[15] */ -1.93472010208f;
	// End of neuron

	a[0] = m[0] + m[8];
	a[1] = m[1] + m[9];
	a[2] = m[2] + m[10];
	a[3] = m[3] + m[11];
	a[4] = m[4] + m[12];
	a[5] = m[5] + m[13];
	a[6] = m[6] + m[14];
	a[7] = m[7] + m[15];
	// End of neuron

	a[0] = a[0] + /* w[16] */ -0.574572193048f;
	a[1] = a[1] + /* w[17] */ 4.47768072844f;
	a[2] = a[2] + /* w[18] */ -0.403279381759f;
	a[3] = a[3] + /* w[19] */ 0.36128697656f;
	a[4] = a[4] + /* w[20] */ 0.120947451965f;
	a[5] = a[5] + /* w[21] */ 0.423705678114f;
	a[6] = a[6] + /* w[22] */ 0.45418197324f;
	a[7] = a[7] + /* w[23] */ 1.5779174415f;
	// End of neuron

	n[0] = SIGMOID_SYMMETRIC(a[0], 0.5f);
	n[1] = SIGMOID_SYMMETRIC(a[1], 0.5f);
	n[2] = SIGMOID_SYMMETRIC(a[2], 0.5f);
	n[3] = SIGMOID_SYMMETRIC(a[3], 0.5f);
	n[4] = SIGMOID_SYMMETRIC(a[4], 0.5f);
	n[5] = SIGMOID_SYMMETRIC(a[5], 0.5f);
	n[6] = SIGMOID_SYMMETRIC(a[6], 0.5f);
	n[7] = SIGMOID_SYMMETRIC(a[7], 0.5f);
	// End of layer

	m[0] = n[0] * /* w[24] */ 2.05729447906f;
	m[1] = n[0] * /* w[25] */ -3.04649991924f;
	// End of neuron

	m[2] = n[1] * /* w[26] */ -0.67993531034f;
	m[3] = n[1] * /* w[27] */ 2.01102189574f;
	// End of neuron

	m[4] = n[2] * /* w[28] */ 6.47247669934f;
	m[5] = n[2] * /* w[29] */ -10.0636974989f;
	// End of neuron

	m[6] = n[3] * /* w[30] */ -4.80526802013f;
	m[7] = n[3] * /* w[31] */ 8.76108241968f;
	// End of neuron

	m[8] = n[4] * /* w[32] */ 4.04574732792f;
	m[9] = n[4] * /* w[33] */ -4.5726708561f;
	// End of neuron

	m[10] = n[5] * /* w[34] */ 0.0452680650865f;
	m[11] = n[5] * /* w[35] */ -0.423894138827f;
	// End of neuron

	m[12] = n[6] * /* w[36] */ 1.66903985871f;
	m[13] = n[6] * /* w[37] */ 0.642292265877f;
	// End of neuron

	m[14] = n[7] * /* w[38] */ -1.50267461907f;
	m[15] = n[7] * /* w[39] */ 3.65464390027f;
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

	a[0] = a[0] + m[8];
	a[1] = a[1] + m[9];
	// End of neuron

	a[0] = a[0] + m[10];
	a[1] = a[1] + m[11];
	// End of neuron

	a[0] = a[0] + m[12];
	a[1] = a[1] + m[13];
	// End of neuron

	a[0] = a[0] + m[14];
	a[1] = a[1] + m[15];
	// End of neuron

	a[0] = a[0] + /* w[40] */ 3.00110879462f;
	a[1] = a[1] + /* w[41] */ -0.358736777525f;
	// End of neuron

	y[0] = LINEAR(a[0], 0.5f);
	y[1] = LINEAR(a[1], 0.5f);
	// End of layer

	return;
}
