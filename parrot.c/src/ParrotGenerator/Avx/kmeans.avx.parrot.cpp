/*
 * Neural Parrot!
 *
 * Hadi Esmaeilzadeh
 * <hadianeh@cs.washington.edu>
 */

#include "parrot_avx_hwsigmoid.h"

static __m256 _a0 __attribute__ ((aligned(32)));
static __m256 _a0_a1_a2_a3 __attribute__ ((aligned(32)));
static __m256 _a0_a1_a2_a3_a4_a5_a6_a7 __attribute__ ((aligned(32)));
static __m256 _m0 __attribute__ ((aligned(32)));
static __m256 _m0_m1_m2_m3 __attribute__ ((aligned(32)));
static __m256 _m0_m1_m2_m3_m4_m5_m6_m7 __attribute__ ((aligned(32)));
static __m256 _m1 __attribute__ ((aligned(32)));
static __m256 _m12_m13_m14_m15 __attribute__ ((aligned(32)));
static __m256 _m16_m17_m18_m19 __attribute__ ((aligned(32)));
static __m256 _m16_m17_m18_m19_m20_m21_m22_m23 __attribute__ ((aligned(32)));
static __m256 _m2 __attribute__ ((aligned(32)));
static __m256 _m20_m21_m22_m23 __attribute__ ((aligned(32)));
static __m256 _m24_m25_m26_m27 __attribute__ ((aligned(32)));
static __m256 _m24_m25_m26_m27_m28_m29_m30_m31 __attribute__ ((aligned(32)));
static __m256 _m28_m29_m30_m31 __attribute__ ((aligned(32)));
static __m256 _m3 __attribute__ ((aligned(32)));
static __m256 _m32_m33_m34_m35_m36_m37_m38_m39 __attribute__ ((aligned(32)));
static __m256 _m40_m41_m42_m43_m44_m45_m46_m47 __attribute__ ((aligned(32)));
static __m256 _m4_m5_m6_m7 __attribute__ ((aligned(32)));
static __m256 _m8_m9_m10_m11 __attribute__ ((aligned(32)));
static __m256 _m8_m9_m10_m11_m12_m13_m14_m15 __attribute__ ((aligned(32)));
static __m256 _n0 __attribute__ ((aligned(32)));
static __m256 _n0_n0_n0_n0 __attribute__ ((aligned(32)));
static __m256 _n0_n1_n2_n3 __attribute__ ((aligned(32)));
static __m256 _n0_n1_n2_n3_n4_n5_n6_n7 __attribute__ ((aligned(32)));
static __m256 _n1 __attribute__ ((aligned(32)));
static __m256 _n1_n1_n1_n1 __attribute__ ((aligned(32)));
static __m256 _n2 __attribute__ ((aligned(32)));
static __m256 _n2_n2_n2_n2 __attribute__ ((aligned(32)));
static __m256 _n3 __attribute__ ((aligned(32)));
static __m256 _n3_n3_n3_n3 __attribute__ ((aligned(32)));
static __m256 _n4_n4_n4_n4 __attribute__ ((aligned(32)));
static __m256 _n5_n5_n5_n5 __attribute__ ((aligned(32)));
static __m256 _n6_n6_n6_n6 __attribute__ ((aligned(32)));
static __m256 _n7_n7_n7_n7 __attribute__ ((aligned(32)));
static __m256 _w0_w1_w2_w3_w4_w5_w6_w7 __attribute__ ((aligned(32))) = {0.866655980034, 0.345695558088, 2.31617503001, 0.492460777492, 1.3036657387, 3.35469542878, -0.594110602548, -1.76959894956};
static __m256 _w16_w17_w18_w19_w20_w21_w22_w23 __attribute__ ((aligned(32))) = {-0.866398454453, -3.13879194186, 1.67932975747, 0.101845537259, -0.13226645602, 1.26265011453, 0.0284732401624, 1.37232976723};
static __m256 _w24_w25_w26_w27_w28_w29_w30_w31 __attribute__ ((aligned(32))) = {-1.35810947754, 0.135108764301, 2.47422880427, 0.054654096649, 0.750564553282, -2.93674238202, 0.951693968124, 2.17241583617};
static __m256 _w32_w33_w34_w35_w36_w37_w38_w39 __attribute__ ((aligned(32))) = {-3.30171599667, 0.133584404252, 2.34944177652, 0.514852425834, 1.08658398496, -0.011300532391, 1.19634980311, -1.96399577629};
static __m256 _w40_w41_w42_w43_w44_w45_w46_w47 __attribute__ ((aligned(32))) = {0.392524226172, 3.71045587977, 2.70053176134, 1.46984930584, -0.392737576492, -0.784720948063, 0.358038134849, -0.896970258864};
static __m256 _w48_w49_w50_w51_w52_w53_w54_w55 __attribute__ ((aligned(32))) = {-1.15513384954, 0.96868325824, 1.02902508329, -0.892289154361, -0.757361872547, 1.1071031926, -1.03945982391, 1.08950877762};
static __m256 _w56_w57_w58_w59 __attribute__ ((aligned(32))) = {0.761621391236, 0.320990424364, 0.865957384834, 1.13157407705, 0.0, 0.0, 0.0, 0.0};
static __m256 _w60_w61_w62_w63 __attribute__ ((aligned(32))) = {-0.560347164555, -0.588471821947, -0.487433158131, -0.916344054151, 0.0, 0.0, 0.0, 0.0};
static __m256 _w64_w65_w66_w67 __attribute__ ((aligned(32))) = {0.0919554794475, 0.985764312385, 0.905887279991, 0.20153683763, 0.0, 0.0, 0.0, 0.0};
static __m256 _w68_w69_w70_w71 __attribute__ ((aligned(32))) = {0.0236360450953, 0.525493702259, 0.589681045094, -0.171253232015, 0.0, 0.0, 0.0, 0.0};
static __m256 _w72_w73_w74_w75 __attribute__ ((aligned(32))) = {0.282038692673, 0.134861778468, 0.144402025291, 0.276687357796, 0.0, 0.0, 0.0, 0.0};
static __m256 _w76_w77_w78_w79 __attribute__ ((aligned(32))) = {0.0675488686634, -0.666914095171, -0.47078140924, -0.278116656013, 0.0, 0.0, 0.0, 0.0};
static __m256 _w80_w81_w82_w83 __attribute__ ((aligned(32))) = {0.141699341962, -0.0324962582104, 0.108517026868, 0.571597905557, 0.0, 0.0, 0.0, 0.0};
static __m256 _w84_w85_w86_w87 __attribute__ ((aligned(32))) = {-0.188589692259, -0.830935276407, -0.638411981736, 0.368010487726, 0.0, 0.0, 0.0, 0.0};
static __m256 _w88_w89_w90_w91 __attribute__ ((aligned(32))) = {0.621407515063, 0.511771573677, 0.826897536827, 0.798433323089, 0.0, 0.0, 0.0, 0.0};
static __m256 _w8_w9_w10_w11_w12_w13_w14_w15 __attribute__ ((aligned(32))) = {2.83432991093, 0.442350890738, 5.57737233086, 0.0982497522453, -0.208515944301, 0.521292761251, 1.66745790319, 2.56539949792};
static __m256 _w92 __attribute__ ((aligned(32))) = {-0.406290458355, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w93 __attribute__ ((aligned(32))) = {3.21615937935, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w94 __attribute__ ((aligned(32))) = {1.89521738107, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w95 __attribute__ ((aligned(32))) = {1.15726676562, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _w96 __attribute__ ((aligned(32))) = {0.537994813601, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static __m256 _x0_x0_x0_x0_x0_x0_x0_x0 __attribute__ ((aligned(32)));
static __m256 _x1_x1_x1_x1_x1_x1_x1_x1 __attribute__ ((aligned(32)));
static __m256 _x2_x2_x2_x2_x2_x2_x2_x2 __attribute__ ((aligned(32)));
static __m256 _x3_x3_x3_x3_x3_x3_x3_x3 __attribute__ ((aligned(32)));
static __m256 _x4_x4_x4_x4_x4_x4_x4_x4 __attribute__ ((aligned(32)));
static __m256 _x5_x5_x5_x5_x5_x5_x5_x5 __attribute__ ((aligned(32)));
static __m256 _y0 __attribute__ ((aligned(32)));
static float y_tmp[8] __attribute__ ((aligned(32)));

/** 6 -> 8 -> 4 -> 1 */
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

	_a0_a1_a2_a3_a4_a5_a6_a7 = _mm256_add_ps(_a0_a1_a2_a3_a4_a5_a6_a7, _w48_w49_w50_w51_w52_w53_w54_w55);
	// End of neuron add[0][6]

	_n0_n1_n2_n3_n4_n5_n6_n7 = SIGMOID_SYMMETRIC(_a0_a1_a2_a3_a4_a5_a6_a7 /*, _0.5f_0.5f_0.5f_0.5f_0.5f_0.5f_0.5f_0.5f */);
	// End of layer layer[1]
	// ------------------------------

	_n0_n0_n0_n0 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0x00);
	_m0_m1_m2_m3 = _mm256_mul_ps(_n0_n0_n0_n0, _w56_w57_w58_w59);
	// End of neuron mult[1][0]

	_n1_n1_n1_n1 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0x55);
	_m4_m5_m6_m7 = _mm256_mul_ps(_n1_n1_n1_n1, _w60_w61_w62_w63);
	// End of neuron mult[1][1]

	_n2_n2_n2_n2 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0xAA);
	_m8_m9_m10_m11 = _mm256_mul_ps(_n2_n2_n2_n2, _w64_w65_w66_w67);
	// End of neuron mult[1][2]

	_n3_n3_n3_n3 = _mm256_shuffle_ps(_n0_n1_n2_n3_n4_n5_n6_n7, _n0_n1_n2_n3_n4_n5_n6_n7, 0xFF);
	_m12_m13_m14_m15 = _mm256_mul_ps(_n3_n3_n3_n3, _w68_w69_w70_w71);
	// End of neuron mult[1][3]

	_n4_n4_n4_n4 = _mm256_permute2f128_ps(_n0_n0_n0_n0, _n0_n0_n0_n0, 0x11);
	_m16_m17_m18_m19 = _mm256_mul_ps(_n4_n4_n4_n4, _w72_w73_w74_w75);
	// End of neuron mult[1][4]

	_n5_n5_n5_n5 = _mm256_permute2f128_ps(_n1_n1_n1_n1, _n1_n1_n1_n1, 0x11);
	_m20_m21_m22_m23 = _mm256_mul_ps(_n5_n5_n5_n5, _w76_w77_w78_w79);
	// End of neuron mult[1][5]

	_n6_n6_n6_n6 = _mm256_permute2f128_ps(_n2_n2_n2_n2, _n2_n2_n2_n2, 0x11);
	_m24_m25_m26_m27 = _mm256_mul_ps(_n6_n6_n6_n6, _w80_w81_w82_w83);
	// End of neuron mult[1][6]

	_n7_n7_n7_n7 = _mm256_permute2f128_ps(_n3_n3_n3_n3, _n3_n3_n3_n3, 0x11);
	_m28_m29_m30_m31 = _mm256_mul_ps(_n7_n7_n7_n7, _w84_w85_w86_w87);
	// End of neuron mult[1][7]

	_a0_a1_a2_a3 = _mm256_add_ps(_m0_m1_m2_m3, _m4_m5_m6_m7);
	// End of neuron add[1][1]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _m8_m9_m10_m11);
	// End of neuron add[1][2]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _m12_m13_m14_m15);
	// End of neuron add[1][3]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _m16_m17_m18_m19);
	// End of neuron add[1][4]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _m20_m21_m22_m23);
	// End of neuron add[1][5]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _m24_m25_m26_m27);
	// End of neuron add[1][6]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _m28_m29_m30_m31);
	// End of neuron add[1][7]

	_a0_a1_a2_a3 = _mm256_add_ps(_a0_a1_a2_a3, _w88_w89_w90_w91);
	// End of neuron add[1][8]

	_n0_n1_n2_n3 = SIGMOID_SYMMETRIC(_a0_a1_a2_a3 /*, _0.5f_0.5f_0.5f_0.5f */);
	// End of layer layer[2]
	// ------------------------------

	_n0 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0x00);
	_m0 = _mm256_mul_ps(_n0, _w92);
	// End of neuron mult[2][0]

	_n1 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0x55);
	_m1 = _mm256_mul_ps(_n1, _w93);
	// End of neuron mult[2][1]

	_n2 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0xAA);
	_m2 = _mm256_mul_ps(_n2, _w94);
	// End of neuron mult[2][2]

	_n3 = _mm256_shuffle_ps(_n0_n1_n2_n3, _n0_n1_n2_n3, 0xFF);
	_m3 = _mm256_mul_ps(_n3, _w95);
	// End of neuron mult[2][3]

	_a0 = _mm256_add_ps(_m0, _m1);
	// End of neuron add[2][1]

	_a0 = _mm256_add_ps(_a0, _m2);
	// End of neuron add[2][2]

	_a0 = _mm256_add_ps(_a0, _m3);
	// End of neuron add[2][3]

	_a0 = _mm256_add_ps(_a0, _w96);
	// End of neuron add[2][4]

	_y0 = LINEAR(_a0 /*, _0.5f */);
	_mm256_store_ps(&y_tmp[0], _y0);
	y[0] = y_tmp[0];
	// End of layer layer[3]
	// ------------------------------

	return;
}
