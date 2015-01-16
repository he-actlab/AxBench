// Copyright 2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.


//Build Notes: /QxSSE4.1 /O3. Also, include /DWIN32 if building with 32-bit compiler

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <time.h>
#include <math.h>
#include <immintrin.h>

const int defaultBlockCount = 16;
const int defaultLoopCount = 1;
const float g_pi = 3.141592654f;

int g_blockCount = defaultBlockCount;	//This controls how many blocks to transform
int g_loopCount = defaultLoopCount;		//This controls the number of times to transform the blocks

float * inverseData;
float * dctData;
float * inputData;
short * integerDCTData;
short * integerKernelResults;

using namespace std;

//Tangent and Cosine constants
#define BITS_INV_ACC 4
#define SHIFT_INV_ROW 16 - BITS_INV_ACC
#define SHIFT_INV_COL 1 + BITS_INV_ACC
const short IRND_INV_ROW = 1024 * (6 - BITS_INV_ACC);	//1 << (SHIFT_INV_ROW-1)
const short IRND_INV_COL = 16 * (BITS_INV_ACC - 3);		// 1 << (SHIFT_INV_COL-1)
const short IRND_INV_CORR = IRND_INV_COL - 1;			// correction -1.0 and round

__declspec(align(16)) short shortM128_one_corr[8] = {1, 1, 1, 1, 1, 1, 1, 1};
__declspec(align(16)) short shortM128_round_inv_row[8] = {IRND_INV_ROW, 0, IRND_INV_ROW, 0, IRND_INV_ROW, 0, IRND_INV_ROW, 0};
__declspec(align(16)) short shortM128_round_inv_col[8] = {IRND_INV_COL, IRND_INV_COL, IRND_INV_COL, IRND_INV_COL, IRND_INV_COL, IRND_INV_COL, IRND_INV_COL, IRND_INV_COL};
__declspec(align(16)) short shortM128_round_inv_corr[8]= {IRND_INV_CORR, IRND_INV_CORR, IRND_INV_CORR, IRND_INV_CORR, IRND_INV_CORR, IRND_INV_CORR, IRND_INV_CORR, IRND_INV_CORR};
__declspec(align(16)) short shortM128_tg_1_16[8] = {13036, 13036, 13036, 13036, 13036, 13036, 13036, 13036}; // tg * (2<<16) + 0.5
__declspec(align(16)) short shortM128_tg_2_16[8] = {27146, 27146, 27146, 27146, 27146, 27146, 27146, 27146}; // tg * (2<<16) + 0.5
__declspec(align(16)) short shortM128_tg_3_16[8] = {-21746, -21746, -21746, -21746, -21746, -21746, -21746, -21746}; // tg * (2<<16) + 0.5
__declspec(align(16)) short shortM128_cos_4_16[8] = {-19195, -19195, -19195, -19195, -19195, -19195, -19195, -19195};// cos * (2<<16) + 0.5

//-----------------------------------------------------------------------------
// Table for rows 0,4 - constants are multiplied on cos_4_16
// w15 w14 w11 w10 w07 w06 w03 w02
// w29 w28 w25 w24 w21 w20 w17 w16
// w31 w30 w27 w26 w23 w22 w19 w18
//movq -> w05 w04 w01 w00
__declspec(align(16)) short shortM128_tab_i_04[] = {
	16384, 21407, 16384, 8867,
	16384, -8867, 16384, -21407, // w13 w12 w09 w08
	16384, 8867, -16384, -21407, // w07 w06 w03 w02
	-16384, 21407, 16384, -8867, // w15 w14 w11 w10
	22725, 19266, 19266, -4520, // w21 w20 w17 w16
	12873, -22725, 4520, -12873, // w29 w28 w25 w24
	12873, 4520, -22725, -12873, // w23 w22 w19 w18
	4520, 19266, 19266, -22725}; // w31 w30 w27 w26

	// Table for rows 1,7 - constants are multiplied on cos_1_16
//movq -> w05 w04 w01 w00
__declspec(align(16)) short shortM128_tab_i_17[] = {
	22725, 29692, 22725, 12299,
	22725, -12299, 22725, -29692, // w13 w12 w09 w08
	22725, 12299, -22725, -29692, // w07 w06 w03 w02
	-22725, 29692, 22725, -12299, // w15 w14 w11 w10
	31521, 26722, 26722, -6270, // w21 w20 w17 w16
	17855, -31521, 6270, -17855, // w29 w28 w25 w24
	17855, 6270, -31521, -17855, // w23 w22 w19 w18
	6270, 26722, 26722, -31521}; // w31 w30 w27 w26

// Table for rows 2,6 - constants are multiplied on cos_2_16
//movq -> w05 w04 w01 w00
__declspec(align(16)) short shortM128_tab_i_26[] = {
	21407, 27969, 21407, 11585,
	21407, -11585, 21407, -27969, // w13 w12 w09 w08
	21407, 11585, -21407, -27969, // w07 w06 w03 w02
	-21407, 27969, 21407, -11585, // w15 w14 w11 w10
	29692, 25172, 25172, -5906,	// w21 w20 w17 w16
	16819, -29692, 5906, -16819, // w29 w28 w25 w24
	16819, 5906, -29692, -16819, // w23 w22 w19 w18
	5906, 25172, 25172, -29692}; // w31 w30 w27 w26
// Table for rows 3,5 - constants are multiplied on cos_3_16
//movq -> w05 w04 w01 w00
__declspec(align(16)) short shortM128_tab_i_35[] = {
	19266, 25172, 19266, 10426,
	19266, -10426, 19266, -25172, // w13 w12 w09 w08
	19266, 10426, -19266, -25172, // w07 w06 w03 w02
	-19266, 25172, 19266, -10426, // w15 w14 w11 w10
	26722, 22654, 22654, -5315, // w21 w20 w17 w16
	15137, -26722, 5315, -15137, // w29 w28 w25 w24
	15137, 5315, -26722, -15137, // w23 w22 w19 w18
	5315, 22654, 22654, -26722}; // w31 w30 w27 w26

//Functions 
long getTimestamp(void)	{
	
	union cpuTimeStamp	{
		struct	{
			int ls32bits;
			int ms32bits;			
		};
		long timeStamp;
	}	thisTimeStamp;
	
#if	defined (_MSC_VER)
#ifdef WIN32
	//Microsoft compiler is being used
	__asm
	{
		push	eax
		push	edx
		rdtsc
		mov	thisTimeStamp.ls32bits, eax
		mov	thisTimeStamp.ms32bits, edx
		pop		edx
		pop		eax
	};
#else
	__asm
	{
		push	rax
		push	rdx
		rdtsc
		mov	thisTimeStamp.ls32bits, eax
		mov	thisTimeStamp.ms32bits, edx
		pop		rdx
		pop		rax
	};
#endif
#else
	asm volatile
	(
	 "rdtsc;"
	 : "=a" (thisTimeStamp.ls32bits),
	 "=d" (thisTimeStamp.ms32bits)
	 );
#endif	
	//return the timestamp
	return thisTimeStamp.timeStamp;
}

//The SetKernelParameters function is passing the kernel the command line parameters
void SetKernelParameters(int argc, char* argv[])
{
	//Initialization
	g_blockCount = defaultBlockCount;
	g_loopCount = defaultLoopCount;

	switch(argc)	{
	case 1:
		break;
	case 2:

		//first two parameters are the exe name and the number of blocks to transform
		g_blockCount = atoi(argv[1]);
		if(g_blockCount < 1)	{
			cout << "Warning: block count must be positive, setting to " << defaultBlockCount << endl;
			g_blockCount = defaultBlockCount;
		}
		break;
	case 3:

		//first three parameters are the exe name,the number of blocks to transform, 
		//and the number of times to transform the blocks
		g_blockCount = atoi(argv[1]);
		if(g_blockCount < 1)	{
			cout << "Warning: block count must be positive, setting to " << defaultBlockCount << endl;
			g_blockCount = defaultBlockCount;
		}
		g_loopCount = atoi(argv[2]);
		if(g_loopCount < 1)	{
			cout << "Warning: iteration count must be positive, setting to " << defaultLoopCount << endl;
			g_loopCount = defaultLoopCount;
		}
		break;
	default:
		cout << "idct_short_SSE Usage" << endl;
		cout << " The performance of a short integer inverse discrete cosine transform is performed using Intel SSE instructions" << endl;
		cout << " Scalar code performs a DCT on randomly initialized data, the IDCT routines" << endl;
		cout << " transform the data. Results are compared to the output of a scalar IDCT function." << endl;
		cout << " The optional command line arguments specifies the block count and iteration count." << endl;
		cout << " Note: Validation against IEEE 1180-1900 requires testing 10000 blocks." << endl;
		cout << " The defaults are block count = " << defaultBlockCount << ", iteration count = " << defaultLoopCount << "." << endl;	
		cout << " idct_short_SSE blockCount iterationCount" << endl;
		for(int i = 0; i < argc; i++)	{
			cout << "arg = " << argv[i] << endl;
		}
		exit(1);
		break;
	}

	return;
}

void initDataWithZero(void)
{
	const int blockSize = 8*8;

	//Initialize the blocks
	for (int i = 0; i < g_blockCount; i++)	{
		for(int j = 0; j < blockSize; j++)	{

			inputData[i*blockSize + j] = 0L;
		}
	}
}

void initData(long L, long H)
{
	const int blockSize = 8*8;

	//Random number generator as specified in IEEE Std 1180-1990
	static long randx = 1;
	static double z = (double) 0x7fffffff;

	for (int ii = 0; ii < g_blockCount; ii++)	{
		for(int jj = 0; jj < blockSize; jj++)	{

			long i, j;
			double x;

			randx = (randx*1103515245) + 12345;
			i = randx & 0x7ffffffe;				//Keep 30 bits
			x = ((double) i)/z;					//Range 0 to 0.99999...
			x *= (L + H + 1);					//Range 0 to < L+H+1
			j = x;								//Truncate to integer
			inputData[ii*blockSize + jj] = (float) (j - L);
		}
	}
}

//Memory allocations and buffer initialization
int AllocateAndInitBuffers(int AlignmentBytes)
{
	//calculate necessary buffer size
	const int blockSize = 8*8;
	int iBufferSizeBytes = g_blockCount*blockSize*sizeof(float);

	//should be 16 byte alignment for SSE
	inputData = (float *) _mm_malloc(iBufferSizeBytes, AlignmentBytes); 
	inverseData = (float *) _mm_malloc(iBufferSizeBytes, AlignmentBytes); 
	dctData = (float *) _mm_malloc(iBufferSizeBytes, AlignmentBytes); 
	integerDCTData = (short *) _mm_malloc(g_blockCount*blockSize*sizeof(short), AlignmentBytes); 
	integerKernelResults = (short *) _mm_malloc(g_blockCount*blockSize*sizeof(short), AlignmentBytes); 

	return 0;
}

//Memory free
int DeAllocateBuffers()
{
	//free allocated buffers
	_mm_free(inputData);
	_mm_free(inverseData);
	_mm_free(dctData);
	_mm_free(integerDCTData);
	_mm_free(integerKernelResults);

	return 0;
}

//The SSE short integer IDCT
void idctSSEShort()
{

	__m128i r_xmm0, r_xmm4;
	__m128i r_xmm1, r_xmm2, r_xmm3, r_xmm5, r_xmm6, r_xmm7;
	__m128i row0, row1, row2, row3, row4, row5, row6, row7;
	short * pInput;
	short * pOutput;
	short * pTab_i_04 = shortM128_tab_i_04;
	short * pTab_i_26 = shortM128_tab_i_26;
	const int blockSize = 8*8;

	long startTime = getTimestamp();

	//Transform all the blocks N times
	//The iteration count is a command line option
	const int maxLoopCount = g_loopCount;
	for(int loopCount = 0; loopCount < maxLoopCount; loopCount++)	{

		//Operate on all the blocks
		for(int i = 0; i < g_blockCount; i++)	{
		
			//Get pointers for this input and output
			pInput = &integerDCTData[i*blockSize];
			pOutput = &integerKernelResults[i*blockSize];
			pTab_i_04 = shortM128_tab_i_04;
			pTab_i_26 = shortM128_tab_i_26;

			//Row 1 and Row 3
			r_xmm0 = _mm_load_si128((__m128i *) pInput);
			r_xmm4 = _mm_load_si128((__m128i *) (&pInput[2*8]));

			// *** Work on the data in xmm0
			//low shuffle mask = 0xd8 = 11 01 10 00
			//get short 2 and short 0 into ls 32-bits
			r_xmm0 = _mm_shufflelo_epi16(r_xmm0, 0xd8);

			// copy short 2 and short 0 to all locations
			r_xmm1 = _mm_shuffle_epi32(r_xmm0, 0);
		
			// add to those copies
			r_xmm1 = _mm_madd_epi16(r_xmm1, *((__m128i *) pTab_i_04));

			// shuffle mask = 0x55 = 01 01 01 01
			// copy short 3 and short 1 to all locations
			r_xmm3 = _mm_shuffle_epi32(r_xmm0, 0x55);
		
			// high shuffle mask = 0xd8 = 11 01 10 00
			// get short 6 and short 4 into bit positions 64-95
			// get short 7 and short 5 into bit positions 96-127
			r_xmm0 = _mm_shufflehi_epi16(r_xmm0, 0xd8);
		
			// add to short 3 and short 1
			r_xmm3 = _mm_madd_epi16(r_xmm3, *((__m128i *) &pTab_i_04[16]));
		
			// shuffle mask = 0xaa = 10 10 10 10
			// copy short 6 and short 4 to all locations
			r_xmm2 = _mm_shuffle_epi32(r_xmm0, 0xaa);
		
			// shuffle mask = 0xaa = 11 11 11 11
			// copy short 7 and short 5 to all locations
			r_xmm0 = _mm_shuffle_epi32(r_xmm0, 0xff);
		
			// add to short 6 and short 4
			r_xmm2 = _mm_madd_epi16(r_xmm2, *((__m128i *) &pTab_i_04[8])); 
		
			// *** Work on the data in xmm4
			// high shuffle mask = 0xd8 11 01 10 00
			// get short 6 and short 4 into bit positions 64-95
			// get short 7 and short 5 into bit positions 96-127
			r_xmm4 = _mm_shufflehi_epi16(r_xmm4, 0xd8);
		
			// (xmm0 short 2 and short 0 plus pSi) + some constants
			r_xmm1 = _mm_add_epi32(r_xmm1, *((__m128i *) shortM128_round_inv_row));
			r_xmm4 = _mm_shufflelo_epi16(r_xmm4, 0xd8);
			r_xmm0 = _mm_madd_epi16(r_xmm0, *((__m128i *) &pTab_i_04[24]));
			r_xmm5 = _mm_shuffle_epi32(r_xmm4, 0);
			r_xmm6 = _mm_shuffle_epi32(r_xmm4, 0xaa);
			r_xmm5 = _mm_madd_epi16(r_xmm5, *((__m128i *) &shortM128_tab_i_26[0]));
			r_xmm1 = _mm_add_epi32(r_xmm1, r_xmm2);
			r_xmm2 = r_xmm1;
			r_xmm7 = _mm_shuffle_epi32(r_xmm4, 0x55);
			r_xmm6 = _mm_madd_epi16(r_xmm6, *((__m128i *) &shortM128_tab_i_26[8])); 
			r_xmm0 = _mm_add_epi32(r_xmm0, r_xmm3);
			r_xmm4 = _mm_shuffle_epi32(r_xmm4, 0xff);
			r_xmm2 = _mm_sub_epi32(r_xmm2, r_xmm0);
			r_xmm7 = _mm_madd_epi16(r_xmm7, *((__m128i *) &shortM128_tab_i_26[16])); 
			r_xmm0 = _mm_add_epi32(r_xmm0, r_xmm1);
			r_xmm2 = _mm_srai_epi32(r_xmm2, 12);
			r_xmm5 = _mm_add_epi32(r_xmm5, *((__m128i *) shortM128_round_inv_row));
			r_xmm4 = _mm_madd_epi16(r_xmm4, *((__m128i *) &shortM128_tab_i_26[24]));
			r_xmm5 = _mm_add_epi32(r_xmm5, r_xmm6);
			r_xmm6 = r_xmm5;
			r_xmm0 = _mm_srai_epi32(r_xmm0, 12);
			r_xmm2 = _mm_shuffle_epi32(r_xmm2, 0x1b);
			row0 = _mm_packs_epi32(r_xmm0, r_xmm2);
			r_xmm4 = _mm_add_epi32(r_xmm4, r_xmm7);
			r_xmm6 = _mm_sub_epi32(r_xmm6, r_xmm4);
			r_xmm4 = _mm_add_epi32(r_xmm4, r_xmm5);
			r_xmm6 = _mm_srai_epi32(r_xmm6, 12);
			r_xmm4 = _mm_srai_epi32(r_xmm4, 12);
			r_xmm6 = _mm_shuffle_epi32(r_xmm6, 0x1b);
			row2 = _mm_packs_epi32(r_xmm4, r_xmm6);

			//Row 5 and row 7
			r_xmm0 = _mm_load_si128((__m128i *) (&pInput[4*8]));
			r_xmm4 = _mm_load_si128((__m128i *) (&pInput[6*8]));

			r_xmm0 = _mm_shufflelo_epi16(r_xmm0, 0xd8);
			r_xmm1 = _mm_shuffle_epi32(r_xmm0, 0);
			r_xmm1 = _mm_madd_epi16(r_xmm1, *((__m128i *) pTab_i_04));
			r_xmm3 = _mm_shuffle_epi32(r_xmm0, 0x55);
			r_xmm0 = _mm_shufflehi_epi16(r_xmm0, 0xd8);
			r_xmm3 = _mm_madd_epi16(r_xmm3, *((__m128i *) &pTab_i_04[16]));
			r_xmm2 = _mm_shuffle_epi32(r_xmm0, 0xaa);
			r_xmm0 = _mm_shuffle_epi32(r_xmm0, 0xff);
			r_xmm2 = _mm_madd_epi16(r_xmm2, *((__m128i *) &pTab_i_04[8])); 
			r_xmm4 = _mm_shufflehi_epi16(r_xmm4, 0xd8);
			r_xmm1 = _mm_add_epi32(r_xmm1, *((__m128i *) shortM128_round_inv_row));
			r_xmm4 = _mm_shufflelo_epi16(r_xmm4, 0xd8);
			r_xmm0 = _mm_madd_epi16(r_xmm0, *((__m128i *) &pTab_i_04[24]));
			r_xmm5 = _mm_shuffle_epi32(r_xmm4, 0);
			r_xmm6 = _mm_shuffle_epi32(r_xmm4, 0xaa);
			r_xmm5 = _mm_madd_epi16(r_xmm5, *((__m128i *) &shortM128_tab_i_26[0]));
			r_xmm1 = _mm_add_epi32(r_xmm1, r_xmm2);
			r_xmm2 = r_xmm1;
			r_xmm7 = _mm_shuffle_epi32(r_xmm4, 0x55);
			r_xmm6 = _mm_madd_epi16(r_xmm6, *((__m128i *) &shortM128_tab_i_26[8])); 
			r_xmm0 = _mm_add_epi32(r_xmm0, r_xmm3);
			r_xmm4 = _mm_shuffle_epi32(r_xmm4, 0xff);
			r_xmm2 = _mm_sub_epi32(r_xmm2, r_xmm0);
			r_xmm7 = _mm_madd_epi16(r_xmm7, *((__m128i *) &shortM128_tab_i_26[16])); 
			r_xmm0 = _mm_add_epi32(r_xmm0, r_xmm1);
			r_xmm2 = _mm_srai_epi32(r_xmm2, 12);
			r_xmm5 = _mm_add_epi32(r_xmm5, *((__m128i *) shortM128_round_inv_row));
			r_xmm4 = _mm_madd_epi16(r_xmm4, *((__m128i *) &shortM128_tab_i_26[24]));
			r_xmm5 = _mm_add_epi32(r_xmm5, r_xmm6);
			r_xmm6 = r_xmm5;
			r_xmm0 = _mm_srai_epi32(r_xmm0, 12);
			r_xmm2 = _mm_shuffle_epi32(r_xmm2, 0x1b);
			row4 = _mm_packs_epi32(r_xmm0, r_xmm2);
			r_xmm4 = _mm_add_epi32(r_xmm4, r_xmm7);
			r_xmm6 = _mm_sub_epi32(r_xmm6, r_xmm4);
			r_xmm4 = _mm_add_epi32(r_xmm4, r_xmm5);
			r_xmm6 = _mm_srai_epi32(r_xmm6, 12);
			r_xmm4 = _mm_srai_epi32(r_xmm4, 12);
			r_xmm6 = _mm_shuffle_epi32(r_xmm6, 0x1b);
			row6 = _mm_packs_epi32(r_xmm4, r_xmm6);

			//Row 4 and row 2
			pTab_i_04 = shortM128_tab_i_35;
			pTab_i_26 = shortM128_tab_i_17;
			r_xmm0 = _mm_load_si128((__m128i *) (&pInput[3*8]));
			r_xmm4 = _mm_load_si128((__m128i *) (&pInput[1*8]));

			r_xmm0 = _mm_shufflelo_epi16(r_xmm0, 0xd8);
			r_xmm1 = _mm_shuffle_epi32(r_xmm0, 0);
			r_xmm1 = _mm_madd_epi16(r_xmm1, *((__m128i *) pTab_i_04));
			r_xmm3 = _mm_shuffle_epi32(r_xmm0, 0x55);
			r_xmm0 = _mm_shufflehi_epi16(r_xmm0, 0xd8);
			r_xmm3 = _mm_madd_epi16(r_xmm3, *((__m128i *) &pTab_i_04[16]));
			r_xmm2 = _mm_shuffle_epi32(r_xmm0, 0xaa);
			r_xmm0 = _mm_shuffle_epi32(r_xmm0, 0xff);
			r_xmm2 = _mm_madd_epi16(r_xmm2, *((__m128i *) &pTab_i_04[8])); 
			r_xmm4 = _mm_shufflehi_epi16(r_xmm4, 0xd8);
			r_xmm1 = _mm_add_epi32(r_xmm1, *((__m128i *) shortM128_round_inv_row));
			r_xmm4 = _mm_shufflelo_epi16(r_xmm4, 0xd8);
			r_xmm0 = _mm_madd_epi16(r_xmm0, *((__m128i *) &pTab_i_04[24]));
			r_xmm5 = _mm_shuffle_epi32(r_xmm4, 0);
			r_xmm6 = _mm_shuffle_epi32(r_xmm4, 0xaa);
			r_xmm5 = _mm_madd_epi16(r_xmm5, *((__m128i *) &pTab_i_26[0]));
			r_xmm1 = _mm_add_epi32(r_xmm1, r_xmm2);
			r_xmm2 = r_xmm1;
			r_xmm7 = _mm_shuffle_epi32(r_xmm4, 0x55);
			r_xmm6 = _mm_madd_epi16(r_xmm6, *((__m128i *) &pTab_i_26[8])); 
			r_xmm0 = _mm_add_epi32(r_xmm0, r_xmm3);
			r_xmm4 = _mm_shuffle_epi32(r_xmm4, 0xff);
			r_xmm2 = _mm_sub_epi32(r_xmm2, r_xmm0);
			r_xmm7 = _mm_madd_epi16(r_xmm7, *((__m128i *) &pTab_i_26[16])); 
			r_xmm0 = _mm_add_epi32(r_xmm0, r_xmm1);
			r_xmm2 = _mm_srai_epi32(r_xmm2, 12);
			r_xmm5 = _mm_add_epi32(r_xmm5, *((__m128i *) shortM128_round_inv_row));
			r_xmm4 = _mm_madd_epi16(r_xmm4, *((__m128i *) &pTab_i_26[24]));
			r_xmm5 = _mm_add_epi32(r_xmm5, r_xmm6);
			r_xmm6 = r_xmm5;
			r_xmm0 = _mm_srai_epi32(r_xmm0, 12);
			r_xmm2 = _mm_shuffle_epi32(r_xmm2, 0x1b);
			row3 = _mm_packs_epi32(r_xmm0, r_xmm2);
			r_xmm4 = _mm_add_epi32(r_xmm4, r_xmm7);
			r_xmm6 = _mm_sub_epi32(r_xmm6, r_xmm4);
			r_xmm4 = _mm_add_epi32(r_xmm4, r_xmm5);
			r_xmm6 = _mm_srai_epi32(r_xmm6, 12);
			r_xmm4 = _mm_srai_epi32(r_xmm4, 12);
			r_xmm6 = _mm_shuffle_epi32(r_xmm6, 0x1b);
			row1 = _mm_packs_epi32(r_xmm4, r_xmm6);

			//Row 6 and row 8
			r_xmm0 = _mm_load_si128((__m128i *) (&pInput[5*8]));
			r_xmm4 = _mm_load_si128((__m128i *) (&pInput[7*8]));

			r_xmm0 = _mm_shufflelo_epi16(r_xmm0, 0xd8);
			r_xmm1 = _mm_shuffle_epi32(r_xmm0, 0);
			r_xmm1 = _mm_madd_epi16(r_xmm1, *((__m128i *) pTab_i_04));
			r_xmm3 = _mm_shuffle_epi32(r_xmm0, 0x55);
			r_xmm0 = _mm_shufflehi_epi16(r_xmm0, 0xd8);
			r_xmm3 = _mm_madd_epi16(r_xmm3, *((__m128i *) &pTab_i_04[16]));
			r_xmm2 = _mm_shuffle_epi32(r_xmm0, 0xaa);
			r_xmm0 = _mm_shuffle_epi32(r_xmm0, 0xff);
			r_xmm2 = _mm_madd_epi16(r_xmm2, *((__m128i *) &pTab_i_04[8])); 
			r_xmm4 = _mm_shufflehi_epi16(r_xmm4, 0xd8);
			r_xmm1 = _mm_add_epi32(r_xmm1, *((__m128i *) shortM128_round_inv_row));
			r_xmm4 = _mm_shufflelo_epi16(r_xmm4, 0xd8);
			r_xmm0 = _mm_madd_epi16(r_xmm0, *((__m128i *) &pTab_i_04[24]));
			r_xmm5 = _mm_shuffle_epi32(r_xmm4, 0);
			r_xmm6 = _mm_shuffle_epi32(r_xmm4, 0xaa);
			r_xmm5 = _mm_madd_epi16(r_xmm5, *((__m128i *) &pTab_i_26[0]));
			r_xmm1 = _mm_add_epi32(r_xmm1, r_xmm2);
			r_xmm2 = r_xmm1;
			r_xmm7 = _mm_shuffle_epi32(r_xmm4, 0x55);
			r_xmm6 = _mm_madd_epi16(r_xmm6, *((__m128i *) &pTab_i_26[8])); 
			r_xmm0 = _mm_add_epi32(r_xmm0, r_xmm3);
			r_xmm4 = _mm_shuffle_epi32(r_xmm4, 0xff);
			r_xmm2 = _mm_sub_epi32(r_xmm2, r_xmm0);
			r_xmm7 = _mm_madd_epi16(r_xmm7, *((__m128i *) &pTab_i_26[16])); 
			r_xmm0 = _mm_add_epi32(r_xmm0, r_xmm1);
			r_xmm2 = _mm_srai_epi32(r_xmm2, 12);
			r_xmm5 = _mm_add_epi32(r_xmm5, *((__m128i *) shortM128_round_inv_row));
			r_xmm4 = _mm_madd_epi16(r_xmm4, *((__m128i *) &pTab_i_26[24]));
			r_xmm5 = _mm_add_epi32(r_xmm5, r_xmm6);
			r_xmm6 = r_xmm5;
			r_xmm0 = _mm_srai_epi32(r_xmm0, 12);
			r_xmm2 = _mm_shuffle_epi32(r_xmm2, 0x1b);
			row5 = _mm_packs_epi32(r_xmm0, r_xmm2);
			r_xmm4 = _mm_add_epi32(r_xmm4, r_xmm7);
			r_xmm6 = _mm_sub_epi32(r_xmm6, r_xmm4);
			r_xmm4 = _mm_add_epi32(r_xmm4, r_xmm5);
			r_xmm6 = _mm_srai_epi32(r_xmm6, 12);
			r_xmm4 = _mm_srai_epi32(r_xmm4, 12);
			r_xmm6 = _mm_shuffle_epi32(r_xmm6, 0x1b);
			row7 = _mm_packs_epi32(r_xmm4, r_xmm6);

			r_xmm1 = _mm_load_si128((__m128i *) shortM128_tg_3_16);
			r_xmm2 = row5;
			r_xmm3 = row3;
			r_xmm0 = _mm_mulhi_epi16(row5, r_xmm1);

			r_xmm1 = _mm_mulhi_epi16(r_xmm1, r_xmm3);
			r_xmm5 = _mm_load_si128((__m128i *) shortM128_tg_1_16);
			r_xmm6 = row7;
			r_xmm4 = _mm_mulhi_epi16(row7, r_xmm5);

			r_xmm0 = _mm_adds_epi16(r_xmm0, r_xmm2);
			r_xmm5 = _mm_mulhi_epi16(r_xmm5, row1);
			r_xmm1 = _mm_adds_epi16(r_xmm1, r_xmm3);
			r_xmm7 = row6;

			r_xmm0 = _mm_adds_epi16(r_xmm0, r_xmm3);
			r_xmm3 = _mm_load_si128((__m128i *) shortM128_tg_2_16);
			r_xmm2 = _mm_subs_epi16(r_xmm2, r_xmm1);
			r_xmm7 = _mm_mulhi_epi16(r_xmm7, r_xmm3);
			r_xmm1 = r_xmm0;
			r_xmm3 = _mm_mulhi_epi16(r_xmm3, row2);
			r_xmm5 = _mm_subs_epi16(r_xmm5, r_xmm6);
			r_xmm4 = _mm_adds_epi16(r_xmm4, row1);
			r_xmm0 = _mm_adds_epi16(r_xmm0, r_xmm4);
			r_xmm0 = _mm_adds_epi16(r_xmm0, *((__m128i *) shortM128_one_corr));
			r_xmm4 = _mm_subs_epi16(r_xmm4, r_xmm1);
			r_xmm6 = r_xmm5;
			r_xmm5 = _mm_subs_epi16(r_xmm5, r_xmm2);
			r_xmm5 = _mm_adds_epi16(r_xmm5, *((__m128i *) shortM128_one_corr));
			r_xmm6 = _mm_adds_epi16(r_xmm6, r_xmm2);

			//Intermediate results, needed later
			__m128i temp3, temp7;
			temp7 = r_xmm0;

			r_xmm1 = r_xmm4;
			r_xmm0 = _mm_load_si128((__m128i *) shortM128_cos_4_16);
			r_xmm4 = _mm_adds_epi16(r_xmm4, r_xmm5);
			r_xmm2 = _mm_load_si128((__m128i *) shortM128_cos_4_16);
			r_xmm2 = _mm_mulhi_epi16(r_xmm2, r_xmm4);

			//Intermediate results, needed later
			temp3 = r_xmm6;

			r_xmm1 = _mm_subs_epi16(r_xmm1, r_xmm5);
			r_xmm7 = _mm_adds_epi16(r_xmm7, row2);
			r_xmm3 = _mm_subs_epi16(r_xmm3, row6);
			r_xmm6 = row0;
			r_xmm0 = _mm_mulhi_epi16(r_xmm0, r_xmm1);
			r_xmm5 = row4;
			r_xmm5 = _mm_adds_epi16(r_xmm5, r_xmm6);
			r_xmm6 = _mm_subs_epi16(r_xmm6, row4);
			r_xmm4 = _mm_adds_epi16(r_xmm4, r_xmm2);

			r_xmm4 = _mm_or_si128(r_xmm4, *((__m128i *) shortM128_one_corr));
			r_xmm0 = _mm_adds_epi16(r_xmm0, r_xmm1);
			r_xmm0 = _mm_or_si128(r_xmm0, *((__m128i *) shortM128_one_corr));

			r_xmm2 = r_xmm5;
			r_xmm5 = _mm_adds_epi16(r_xmm5, r_xmm7);
			r_xmm1 = r_xmm6;
			r_xmm5 = _mm_adds_epi16(r_xmm5, *((__m128i *) shortM128_round_inv_col));
			r_xmm2 = _mm_subs_epi16(r_xmm2, r_xmm7);
			r_xmm7 = temp7;
			r_xmm6 = _mm_adds_epi16(r_xmm6, r_xmm3);
			r_xmm6 = _mm_adds_epi16(r_xmm6, *((__m128i *) shortM128_round_inv_col));
			r_xmm7 = _mm_adds_epi16(r_xmm7, r_xmm5);
			r_xmm7 = _mm_srai_epi16(r_xmm7, SHIFT_INV_COL);
			r_xmm1 = _mm_subs_epi16(r_xmm1, r_xmm3);
			r_xmm1 = _mm_adds_epi16(r_xmm1, *((__m128i *) shortM128_round_inv_corr));
			r_xmm3 = r_xmm6;
			r_xmm2 = _mm_adds_epi16(r_xmm2, *((__m128i *) shortM128_round_inv_corr));
			r_xmm6 = _mm_adds_epi16(r_xmm6, r_xmm4);

			//Store results for row 0
			_mm_store_si128((__m128i *) pOutput, r_xmm7);  

			r_xmm6 = _mm_srai_epi16(r_xmm6, SHIFT_INV_COL);
			r_xmm7 = r_xmm1;
			r_xmm1 = _mm_adds_epi16(r_xmm1, r_xmm0);

			//Store results for row 1
			_mm_store_si128((__m128i *) (&pOutput[1*8]), r_xmm6); 

			r_xmm1 = _mm_srai_epi16(r_xmm1, SHIFT_INV_COL);
			r_xmm6 = temp3;
			r_xmm7 = _mm_subs_epi16(r_xmm7, r_xmm0);
			r_xmm7 = _mm_srai_epi16(r_xmm7, SHIFT_INV_COL);

			//Store results for row 2
			_mm_store_si128((__m128i *) (&pOutput[2*8]), r_xmm1); 

			r_xmm5 = _mm_subs_epi16(r_xmm5, temp7); 
			r_xmm5 = _mm_srai_epi16(r_xmm5, SHIFT_INV_COL);

			//Store results for row 7
			_mm_store_si128((__m128i *) (&pOutput[7*8]), r_xmm5); 

			r_xmm3 = _mm_subs_epi16(r_xmm3, r_xmm4);
			r_xmm6 = _mm_adds_epi16(r_xmm6, r_xmm2);
			r_xmm2 = _mm_subs_epi16(r_xmm2, temp3); 
			r_xmm6 = _mm_srai_epi16(r_xmm6, SHIFT_INV_COL);
			r_xmm2 = _mm_srai_epi16(r_xmm2, SHIFT_INV_COL);

			//Store results for row 3
			_mm_store_si128((__m128i *) (&pOutput[3*8]), r_xmm6); 

			r_xmm3 = _mm_srai_epi16(r_xmm3, SHIFT_INV_COL);

			//Store results for rows 4, 5, and 6
			_mm_store_si128((__m128i *) (&pOutput[4*8]), r_xmm2);
			_mm_store_si128((__m128i *) (&pOutput[5*8]), r_xmm7);
			_mm_store_si128((__m128i *) (&pOutput[6*8]), r_xmm3);
		}	
	}

	long duration = getTimestamp() - startTime;
	cout << "SSE (short) Timestamp = " << duration << endl;
}

bool ValidateIEEE(double errorAccumulator, const int *pPixelErrors, const int *pPixelErrorsSquared, const char *pMessage)
{
	bool result = true;
	const int blockSize = 8*8;

	//Calculate the overall mean error
	double denominatorOverall = (double) (blockSize*g_blockCount);
	double omeMax = 0.0015;
	double ome = errorAccumulator/denominatorOverall;
	cout << pMessage << ": Overall mean error ";
	if(ome > omeMax)	{

		//Overall mean error exceeds IEEE spec
		cout << "fails spec, ome = " << ome << ", must not exceed " << omeMax << endl;
	}
	else	{

		//Overall mean square error passes IEEE spec
		cout << "passes spec, ome = " << ome << endl;
	}

	//Verify mean error and mean square error for each pixel, and accumulate overall mean square error
	double pmse, pme;
	double overallMSE = 0.0;
	double denominatorPixel = (double) g_blockCount;
	double mePixelMax = 0.015;		//Max mean error for a pixel
	double msePixelMax = 0.06;		//Max mean square error for a pixel
	double mseOverallMax = 0.02;	//Max overall mean square error for all blocks/pixels

	//Verify for all blocks
	for(int i = 0; i < blockSize; i++)	{

		//Validate mean error for this pixel
		double pe = (double) pPixelErrors[i];
		double pme = pe/denominatorPixel;
		if( pme > mePixelMax)	{
			
			cout << " Pixel (" << i/8 << "," << i%8 << ") mean error fails spec, me = " << pme << ", must not exceed " << mePixelMax << endl;
		}

		//Get the pixel mean square error
		double peSquared = (double) pPixelErrorsSquared[i];

		//Validate mse for this pixel
		if((peSquared/denominatorPixel) > msePixelMax)	{
			
			cout << " Pixel (" << i/8 << "," << i%8 << ") mean square error fails spec, mse = " << peSquared/denominatorPixel << ", must not exceed " << msePixelMax << endl;
		}

		//accumulate overall mse
		overallMSE += peSquared;
	}

	//Validate overall mse for this pixel
	if((overallMSE/denominatorOverall) > mseOverallMax)	{
		
		cout << " Overall mean square error fails spec, mse = " << overallMSE/denominatorOverall << ", must not exceed " << mseOverallMax << endl;
	}

	return result;
}

//the function should check that the single precision float kernel generated results are valid
bool ValidateResult()
{
	int errorCount = 0;
	bool result = true;
	const int blockSize = 8*8;
	const int maxErrors = 16;
	double errorAccumulator = 0.0;
	int pixelError[blockSize];
	int pixelErrorSquared[blockSize];

	//Initialize the mean square error accumulators
	for(int i = 0; i < blockSize; i++)	{
		pixelError[i] = 0;
		pixelErrorSquared[i] = 0;
	}

	for(int i = 0; i < g_blockCount; i++)	{
		for(int j = 0; j < blockSize; j++)	{
			int kernelValue = integerKernelResults[i*blockSize + j];

			double fReferenceValue = inverseData[i*blockSize + j];

			//IEEE 1180-1900 says to round and clip before comparing
			int referenceValue = (int) floor(fReferenceValue + 0.5);
			if(referenceValue < -256)	{
				referenceValue = -256;
			}
			else if(referenceValue > 255)	{
				referenceValue = 255;
			}
			if(kernelValue < -256)	{
				kernelValue = -256;
			}
			else if(kernelValue > 255)	{
				kernelValue = 255;
			}

			if(kernelValue != referenceValue)	{

				//Note the error
				errorAccumulator += (float) (kernelValue - referenceValue);
				errorCount++;
				result = false;
				if(errorCount < maxErrors)	{

					//Report the error
					cout << "Short mismatch at position " << j << " of block " << i << ": input = " << referenceValue << ", kernel result = " << kernelValue << endl;
				}
				//Check that magnitude of the error is not greater than 1
				if((abs(kernelValue - referenceValue) > 1) && (errorCount < maxErrors))	{
					
					cout << "Short error is greater than 1" << endl;
				}

				//Accumulate the pixel error and pixel error squared
				int error = kernelValue - referenceValue;
				pixelError[j] += error;
				pixelErrorSquared[j] += error*error;
			}
		}
	}

	if(false == result)	{

		//Run all the other IEEE Std 1180-1900 conformence tests
		ValidateIEEE(errorAccumulator, pixelError, pixelErrorSquared, "short integer");

		if(maxErrors < errorCount)	{
			cout << "Short integer mismatch reporting stopped at " << maxErrors << ", there were " << errorCount << " mismatches." << endl;
		}
	}
	return result;
}

//the function performs a 2D discrete cosine transform
void discreteCosineTransform()
{
	const int size = 8;
	const int blockSize = 8*8;
	const double pi = 3.141592654;

	//transform all the 8x8 blocks
	for(int block = 0; block < g_blockCount; block++)	{

		//transform one 8x8 block
		for(int v = 0; v < size; v++)	{
			double fv = (double) v;
			double cv;

			//calculate C(v)
			if(v == 0)	{
				cv = 1.0/(sqrt(2.0));
			}
			else	{
				cv = 1.0;
			}
			for(int u = 0; u < size; u++)	{
				double fu = (double) u;
				double cu;

				//calculate C(u)
				if(u == 0)	{
					cu = 1.0/(sqrt(2.0));
				}
				else	{
					cu = 1.0;
				}
				double result = 0.0;
				for(int y = 0; y < size; y++)	{
					double fy = (double) y;

					for(int x = 0; x < size; x++)	{
						double fx = (double) x;
						double inputValue = inputData[block*blockSize + y*size + x];
						
						result += inputValue*cos(((2.0*fx)+1.0)*fu*pi/16.0)*cos(((2.0*fy)+1.0)*fv*pi/16.0);
					}
				}
				result *= 0.25*cv*cu;
				
				//Store this result;
				//IEEE 1180-1190 says to round to integer and clip to -2048 <= result <= 2047
				int iResult;
				iResult = (int) floor(result + 0.5);
				if(iResult < -2048)	{
					iResult = -2048;
				}
				else if(iResult > 2047)	{
					iResult = 2047;
				}
				dctData[block*blockSize + v*size + u] = iResult;
				integerDCTData[block*blockSize + v*size + u] = (short) iResult;
			}
		}
	}
}

//the function performs a 2D inverse discrete cosine transform
void inverseDiscreteCosineTransform()
{

	const int size = 8;
	const int blockSize = 8*8;
	const double pi = 3.141592654;//22.0f/7.0f;

	//transform all the 8x8 blocks
	for(int block = 0; block < g_blockCount; block++)	{

		//transform one 8x8 block
		for(int y = 0; y < size; y++)	{
			double fy = (double) y;

			for(int x = 0; x < size; x++)	{
				double fx = (double) x;

				double result = 0.0;
				for(int v = 0; v < size; v++)	{
					double fv = (double) v;
					double cv;

					//calculate C(v)
					if(v == 0)	{
						cv = 1.0/(sqrt(2.0));
					}
					else	{
						cv = 1.0;
					}
					cv *= 0.5;

					double innerResult = 0.0;

					for(int u = 0; u < size; u++)	{
						double fu = (double) u;
						double cu;

						//calculate C(u)
						if(u == 0)	{
							cu = 1.0/(sqrt(2.0));
						}
						else	{
							cu = 1.0;
						}
						cu *= 0.5;

						double dctValue = dctData[block*blockSize + v*size + u];
						
						innerResult += cu*dctValue*cos(((2.0*fx)+1.0)*fu*pi/16.0)*cos(((2.0*fy)+1.0)*fv*pi/16.0);
					}
					result += cv*innerResult;
				}
				//Store this result;
				inverseData[block*blockSize + y*size + x] = result;
			}
		}
	}
}

bool runTest(long L, long H, char *pMessage)	{

	//Display the test being run
	cout << endl << "Running " << pMessage << endl;

	//Initialize the pseudo-random data
	if((L == 0L) && (H == 0L))	{
		initDataWithZero();
	}
	else	{
		initData(L, H);
	}

	//Produce the reference results via scalar code
	discreteCosineTransform();
	inverseDiscreteCosineTransform();

	//Run the SSE short integer IDCT
	idctSSEShort();

	if(ValidateResult() == true)	{
		cout << "ValidateResult passed for 128 bits, " << pMessage << endl;
	}

	return true;
}

int main (int argc, char * const argv[]) {
    
    cout << "Welcome to short integer IDCT using SSE" << endl;

	SetKernelParameters(argc, (char * *) argv);
	AllocateAndInitBuffers(16);

	//First test, L=256, H = 255 (IEEE 1180-1990)
	runTest(256L, 255L, "test 1");

	//Second test, L=5, H = 5 (IEEE 1180-1990)
	runTest(5L, 5L, "test 2");

	//Third test, L=300, H = 300 (IEEE 1180-1990)
	runTest(300L, 300L, "test 3");

	//Fourth test, L=-255, H = 256 (IEEE 1180-1990)
	runTest(-255L, 256L, "test 4");

	//Fifth test, L=-5, H = 5 (IEEE 1180-1990)
	runTest(-5L, 5L, "test 5");

	//Sixth test, L=-300, H = 300 (IEEE 1180-1990)
	runTest(-300L, 300L, "test 6");

	//Seventh test, All zeros (IEEE 1180-1990)
	runTest(0L, 0L, "zero test");

	DeAllocateBuffers();

	return 0;
}
