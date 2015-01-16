/*
 * Neural Parrot!
 *
 * Hadi Esmaeilzadeh
 * <hadianeh@cs.washington.edu>
 */

#include "parrot_avx_hwsigmoid.h"

static __m256 _a0 __attribute__ ((aligned(32)));
static __m256 _a0_a1_a2_a3_a4_a5_a6_a7 __attribute__ ((aligned(32)));
static __m256 _m0 __attribute__ ((aligned(32)));
static __m256 _m0_m1_m2_m3_m4_m5_m6_m7 __attribute__ ((aligned(32)));
static __m256 _m1 __attribute__ ((aligned(32)));
static __m256 _m16_m17_m18_m19_m20_m21_m22_m23 __attribute__ ((aligned(32)));
static __m256 _m2 __attribute__ ((aligned(32)));
static __m256 _m24_m25_m26_m27_m28_m29_m30_m31 __attribute__ ((aligned(32)));
static __m256 _m3 __attribute__ ((aligned(32)));
static __m256 _m32_m33_m34_m35_m36_m37_m38_m39 __attribute__ ((aligned(32)));
static __m256 _m4 __attribute__ ((aligned(32)));
static __m256 _m40_m41_m42_m43_m44_m45_m46_m47 __attribute__ ((aligned(32)));
static __m256 _m48_m49_m50_m51_m52_m53_m54_m55 __attribute__ ((aligned(32)));
static __m256 _m5 __attribute__ ((aligned(32)));
static __m256 _m56_m57_m58_m59_m60_m61_m62_m63 __attribute__ ((aligned(32)));
static __m256 _m6 __attribute__ ((aligned(32)));
static __m256 _m64_m65_m66_m67_m68_m69_m70_m71 __attribute__ ((aligned(32)));
static __m256 _m7 __attribute__ ((aligned(32)));
static __m256 _m8_m9_m10_m11_m12_m13_m14_m15 __attribute__ ((aligned(32)));
static __m256 _n0 __attribute__ ((aligned(32)));
static __m256 _n0_n1_n2_n3_n4_n5_n6_n7 __attribute__ ((aligned(32)));
static __m256 _n1 __attribute__ ((aligned(32)));
static __m256 _n2 __attribute__ ((aligned(32)));
static __m256 _n3 __attribute__ ((aligned(32)));
static __m256 _n4 __attribute__ ((aligned(32)));
static __m256 _n5 __attribute__ ((aligned(32)));
static __m256 _n6 __attribute__ ((aligned(32)));
static __m256 _n7 __attribute__ ((aligned(32)));
static __m256 _w0_w1_w2_w3_w4_w5_w6_w7 __attribute__ ((aligned(32))) = {-0.868778153393, -2.59073100307, -0.412991545886, 1.06179841974, -0.180986588389, -20.6376913634, -31.3427401458, 4.81852673576};
static __m256 _w16_w17_w18_w19_w20_w21_w22_w23 __attribute__ ((aligned(32))) = {1.07849917023, -2.57690368677, 0.750384476703, 14.060847441, 0.670704656862, 2.83779604079, 0.52832478129, -11.6543208113};
static __m256 _w24_w25_w26_w27_w28_w29_w30_w31 __attribute__ ((aligned(32))) = {-4.34444810646, 2.12816200391, -4.18979325266, -22.8563104891, -4.18004673958, -7.19816319053, -17.0865643717, -16.1455630355};
static __m256 _w32_w33_w34_w35_w36_w37_w38_w39 __attribute__ ((aligned(32))) = {0.0107167340764, 0.634098484844, 0.033151596556, 0.0170320059306, -0.0467279195979, -0.0946925839788, -0.226869553446, 0.62265266015};
static __m256 _w40_w41_w42_w43_w44_w45_w46_w47 __attribute__ ((aligned(32))) = {2.09750776005, -3.87409913305, 1.12151350615, 2.70559674217, 0.943739260038, 8.86852761994, 23.1745546849, -1.25154111618};
static __m256 _w48_w49_w50_w51_w52_w53_w54_w55 __attribute__ ((aligned(32))) = {0.130655182893, 6.93343824625, 0.267395584078, -2.11073587695, 0.205538581334, 0.626912743461, 3.98424913504, 22.3827561854};
static __m256 _w56_w57_w58_w59_w60_w61_w62_w63 __attribute__ ((aligned(32))) = {0.565070758149, 3.17660984743, 0.523438799732, -1.91737267766, 0.227814025876, 8.08137693507, 9.47520177622, 12.8123251806};
static __m256 _w64_w65_w66_w67_w68_w69_w70_w71 __attribute__ ((aligned(32))) = {1.87281403405, 3.29051334914, 0.920256553503, 8.02193844046, 1.1502511644, 15.6874861374, 23.8161870794, 5.31020736442};
static __m256 _w72_w73_w74_w75_w76_w77_w78_w79 __attribute__ ((aligned(32))) = {-0.719199522387, -2.06342122093, -0.60300544722, 1.92784485423, -0.421256623345, 6.4577991592, 2.41647516396, 2.44254634283};
static __m256 _w80 __attribute__ ((aligned(32))) = {16.9426769133, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w81 __attribute__ ((aligned(32))) = {5.84684052145, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w82 __attribute__ ((aligned(32))) = {3.20492325767, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w83 __attribute__ ((aligned(32))) = {-3.63727115847, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w84 __attribute__ ((aligned(32))) = {1.16450402199, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w85 __attribute__ ((aligned(32))) = {-4.32809534521, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w86 __attribute__ ((aligned(32))) = {-2.54277551054, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w87 __attribute__ ((aligned(32))) = {-2.70512210716, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w88 __attribute__ ((aligned(32))) = {2.25468477545, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w8_w9_w10_w11_w12_w13_w14_w15 __attribute__ ((aligned(32))) = {-0.538384432068, -6.54342933337, 0.15246409675, 1.3988915229, 0.231780461643, -8.25298663443, -12.375343661, -17.3711276575};
static __m256 _x0_x0_x0_x0_x0_x0_x0_x0 __attribute__ ((aligned(32)));
static __m256 _x1_x1_x1_x1_x1_x1_x1_x1 __attribute__ ((aligned(32)));
static __m256 _x2_x2_x2_x2_x2_x2_x2_x2 __attribute__ ((aligned(32)));
static __m256 _x3_x3_x3_x3_x3_x3_x3_x3 __attribute__ ((aligned(32)));
static __m256 _x4_x4_x4_x4_x4_x4_x4_x4 __attribute__ ((aligned(32)));
static __m256 _x5_x5_x5_x5_x5_x5_x5_x5 __attribute__ ((aligned(32)));
static __m256 _x6_x6_x6_x6_x6_x6_x6_x6 __attribute__ ((aligned(32)));
static __m256 _x7_x7_x7_x7_x7_x7_x7_x7 __attribute__ ((aligned(32)));
static __m256 _x8_x8_x8_x8_x8_x8_x8_x8 __attribute__ ((aligned(32)));
static __m256 _y0 __attribute__ ((aligned(32)));
static float y_tmp[8] __attribute__ ((aligned(32)));

/** 9 -> 8 -> 1 */
void parrot(float* x, float* y) {
	_x0_x0_x0_x0_x0_x0_x0_x0 = _mm256_broadcast_ss(&x[0]);
	_m0_m1_m2_m3_m4_m5_m6_m7 = _mm256_mul_ps(_x0_x0_x0_x0_x0_x0_x0_x0, _w0_w1_w2_w3_w4_w5_w6_w7);
	// End of neuron mult[0][0]

	_x1_x1_x1_x1_x1_x1_x1_x1 = _mm256_broadcast_ss(&x[1]);
	_m8_m9_m10_m11_m12_m13_m14_m15 = _mm256_mul_ps(_x1_x1_x1_x1_x1_x1_x1_x1, _w8_w9_w10_w11_w12_w13_w14_w15);
	// End of neuron mult[0][1]

	_x2_x2_x2_x2_x2_x2_x2_x2 = _mm256_broadcast_ss(&x[2]);
	_m16_m17_m18_m19_m20_m21_m22_m23 = _mm256_mul_ps(_x2_x2_x2_x2_x2_x2_x2_x2, _w16_w17_w18_w19_w20_w21_w22_w23);
	// End of neuron mult[0][2]

	_x3_x3_x3_x3_x3_x3_x3_x3 = _mm256_broadcast_ss(&x[3]);
	_m24_m25_m26_m27_m28_m29_m30_m31 = _mm256_mul_ps(_x3_x3_x3_x3_x3_x3_x3_x3, _w24_w25_w26_w27_w28_w29_w30_w31);
	// End of neuron mult[0][3]

	_x4_x4_x4_x4_x4_x4_x4_x4 = _mm256_broadcast_ss(&x[4]);
	_m32_m33_m34_m35_m36_m37_m38_m39 = _mm256_mul_ps(_x4_x4_x4_x4_x4_x4_x4_x4, _w32_w33_w34_w35_w36_w37_w38_w39);
	// End of neuron mult[0][4]

	_x5_x5_x5_x5_x5_x5_x5_x5 = _mm256_broadcast_ss(&x[5]);
	_m40_m41_m42_m43_m44_m45_m46_m47 = _mm256_mul_ps(_x5_x5_x5_x5_x5_x5_x5_x5, _w40_w41_w42_w43_w44_w45_w46_w47);
	// End of neuron mult[0][5]

	_x6_x6_x6_x6_x6_x6_x6_x6 = _mm256_broadcast_ss(&x[6]);
	_m48_m49_m50_m51_m52_m53_m54_m55 = _mm256_mul_ps(_x6_x6_x6_x6_x6_x6_x6_x6, _w48_w49_w50_w51_w52_w53_w54_w55);
	// End of neuron mult[0][6]

	_x7_x7_x7_x7_x7_x7_x7_x7 = _mm256_broadcast_ss(&x[7]);
	_m56_m57_m58_m59_m60_m61_m62_m63 = _mm256_mul_ps(_x7_x7_x7_x7_x7_x7_x7_x7, _w56_w57_w58_w59_w60_w61_w62_w63);
	// End of neuron mult[0][7]

	_x8_x8_x8_x8_x8_x8_x8_x8 = _mm256_broadcast_ss(&x[8]);
	_m64_m65_m66_m67_m68_m69_m70_m71 = _mm256_mul_ps(_x8_x8_x8_x8_x8_x8_x8_x8, _w64_w65_w66_w67_w68_w69_w70_w71);
	// End of neuron mult[0][8]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_m0_m1_m2_m3_m4_m5_m6_m7, _m8_m9_m10_m11_m12_m13_m14_m15);
	// End of neuron add[0][1]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _m16_m17_m18_m19_m20_m21_m22_m23);
	// End of neuron add[0][2]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _m24_m25_m26_m27_m28_m29_m30_m31);
	// End of neuron add[0][3]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _m32_m33_m34_m35_m36_m37_m38_m39);
	// End of neuron add[0][4]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _m40_m41_m42_m43_m44_m45_m46_m47);
	// End of neuron add[0][5]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _m48_m49_m50_m51_m52_m53_m54_m55);
	// End of neuron add[0][6]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _m56_m57_m58_m59_m60_m61_m62_m63);
	// End of neuron add[0][7]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _m64_m65_m66_m67_m68_m69_m70_m71);
	// End of neuron add[0][8]

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _w72_w73_w74_w75_w76_w77_w78_w79);
	// End of neuron add[0][9]

	_n0_n1_n2_n3_n4_n5_n6_n7 = SIGMOID(_a0_a1_a2_a3_a4_a5_a6_a7 /*, _0.5f_0.5f_0.5f_0.5f_0.5f_0.5f_0.5f_0.5f */);
	// End of layer layer[1]
	// ------------------------------

	_n0 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0x00);
	_m0 = _mm256_mul_ps(_n0, _w80);
	// End of neuron mult[1][0]

	_n1 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0x55);
	_m1 = _mm256_mul_ps(_n1, _w81);
	// End of neuron mult[1][1]

	_n2 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0xAA);
	_m2 = _mm256_mul_ps(_n2, _w82);
	// End of neuron mult[1][2]

	_n3 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0xFF);
	_m3 = _mm256_mul_ps(_n3, _w83);
	// End of neuron mult[1][3]

	_n4 = _mm256_permute2f128_ps(_n0, _n0, 0x11);
	_m4 = _mm256_mul_ps(_n4, _w84);
	// End of neuron mult[1][4]

	_n5 = _mm256_permute2f128_ps(_n1, _n1, 0x11);
	_m5 = _mm256_mul_ps(_n5, _w85);
	// End of neuron mult[1][5]

	_n6 = _mm256_permute2f128_ps(_n2, _n2, 0x11);
	_m6 = _mm256_mul_ps(_n6, _w86);
	// End of neuron mult[1][6]

	_n7 = _mm256_permute2f128_ps(_n3, _n3, 0x11);
	_m7 = _mm256_mul_ps(_n7, _w87);
	// End of neuron mult[1][7]

	_a0 = _mm256_add_ps(_m0, _m1);
	// End of neuron add[1][1]

	_a0 = _mm256_add_ps(_a0, _m2);
	// End of neuron add[1][2]

	_a0 = _mm256_add_ps(_a0, _m3);
	// End of neuron add[1][3]

	_a0 = _mm256_add_ps(_a0, _m4);
	// End of neuron add[1][4]

	_a0 = _mm256_add_ps(_a0, _m5);
	// End of neuron add[1][5]

	_a0 = _mm256_add_ps(_a0, _m6);
	// End of neuron add[1][6]

	_a0 = _mm256_add_ps(_a0, _m7);
	// End of neuron add[1][7]

	_a0 = _mm256_add_ps(_a0, _w88);
	// End of neuron add[1][8]

	_y0 = SIGMOID(_a0 /*, _0.5f */);
	_mm256_store_ps(&y_tmp[0], _y0);
	y[0] = y_tmp[0];
	// End of layer layer[2]
	// ------------------------------

	return;
}
