/*
 * Neural Parrot!
 *
 * Hadi Esmaeilzadeh
 * <hadianeh@cs.washington.edu>
 */

#include "parrot_cpu_hwsigmoid.h"

static float a[4];
static float m[16];
static float w[38];
static float n[4];

/** 1 -> 4 -> 4 -> 2 */
void parrot(float* x, float* y) {
	m[0] = x[0] * /* w[0] */ 3.67738320954f;
	m[1] = x[0] * /* w[1] */ 4.22986682717f;
	m[2] = x[0] * /* w[2] */ 47.7833731923f;
	m[3] = x[0] * /* w[3] */ -0.289138853497f;
	// End of neuron

	a[0] = m[0] + /* w[4] */ -0.961986326027f;
	a[1] = m[1] + /* w[5] */ -0.918933481023f;
	a[2] = m[2] + /* w[6] */ -1.23577104176f;
	a[3] = m[3] + /* w[7] */ 1.31340158088f;
	// End of neuron

	n[0] = SIGMOID_SYMMETRIC(a[0], 0.5f);
	n[1] = SIGMOID_SYMMETRIC(a[1], 0.5f);
	n[2] = SIGMOID_SYMMETRIC(a[2], 0.5f);
	n[3] = SIGMOID_SYMMETRIC(a[3], 0.5f);
	// End of layer

	m[0] = n[0] * /* w[8] */ -0.904775977622f;
	m[1] = n[0] * /* w[9] */ 1.03219169414f;
	m[2] = n[0] * /* w[10] */ 24.0233096975f;
	m[3] = n[0] * /* w[11] */ -2.17828471916f;
	// End of neuron

	m[4] = n[1] * /* w[12] */ -0.498271330942f;
	m[5] = n[1] * /* w[13] */ -11.4615947491f;
	m[6] = n[1] * /* w[14] */ 0.232458753702f;
	m[7] = n[1] * /* w[15] */ -10.3649629992f;
	// End of neuron

	m[8] = n[2] * /* w[16] */ -0.95811968768f;
	m[9] = n[2] * /* w[17] */ 3.73359122884f;
	m[10] = n[2] * /* w[18] */ -0.812900428706f;
	m[11] = n[2] * /* w[19] */ -1.80856321996f;
	// End of neuron

	m[12] = n[3] * /* w[20] */ 0.3290029729f;
	m[13] = n[3] * /* w[21] */ -0.227268037464f;
	m[14] = n[3] * /* w[22] */ -1.62601703857f;
	m[15] = n[3] * /* w[23] */ 0.64278959825f;
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

	a[0] = a[0] + /* w[24] */ 0.21797340615f;
	a[1] = a[1] + /* w[25] */ 1.24090729436f;
	a[2] = a[2] + /* w[26] */ -0.445888097466f;
	a[3] = a[3] + /* w[27] */ 0.312022772993f;
	// End of neuron

	n[0] = SIGMOID_SYMMETRIC(a[0], 0.5f);
	n[1] = SIGMOID_SYMMETRIC(a[1], 0.5f);
	n[2] = SIGMOID_SYMMETRIC(a[2], 0.5f);
	n[3] = SIGMOID_SYMMETRIC(a[3], 0.5f);
	// End of layer

	m[0] = n[0] * /* w[28] */ 1.07551426483f;
	m[1] = n[0] * /* w[29] */ 7.20508247617f;
	// End of neuron

	m[2] = n[1] * /* w[30] */ -1.69827827052f;
	m[3] = n[1] * /* w[31] */ 1.82238857165f;
	// End of neuron

	m[4] = n[2] * /* w[32] */ 1.66747941318f;
	m[5] = n[2] * /* w[33] */ 0.0189889050494f;
	// End of neuron

	m[6] = n[3] * /* w[34] */ 2.18012104324f;
	m[7] = n[3] * /* w[35] */ 0.344916990516f;
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

	a[0] = a[0] + /* w[36] */ 0.388499855295f;
	a[1] = a[1] + /* w[37] */ 0.507315908911f;
	// End of neuron

	y[0] = SIGMOID_SYMMETRIC(a[0], 0.5f);
	y[1] = SIGMOID_SYMMETRIC(a[1], 0.5f);
	// End of layer

	return;
}
