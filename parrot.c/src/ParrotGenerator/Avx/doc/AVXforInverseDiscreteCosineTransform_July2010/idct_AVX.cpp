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


//Build Notes: /QxAVX /O3. Also, include /DWIN32 if building with 32-bit compiler


#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <time.h>
#include <math.h>
#include <immintrin.h>

const int sizeM8 = 32;

const int defaultBlockCount = 16;
const int defaultLoopCount = 1;
const float g_pi = 3.141592654f;

int g_blockCount = defaultBlockCount;	//This controls how many blocks to transform
int g_loopCount = defaultLoopCount;		//This controls the number of times to transform the blocks

float * inputData;
float * kernelResults;
float * dctData;
float * inverseData;

using namespace std;

//Tangent and Cosine constants
__declspec(align(32)) const __m256 AVX_tg_1_16 = {0.19891357421875f, 0.19891357421875f, 0.19891357421875f, 0.19891357421875f, 0.19891357421875f, 0.19891357421875f, 0.19891357421875f, 0.19891357421875f};
__declspec(align(32)) const __m256 AVX_tg_2_16 = {0.414215087890625, 0.414215087890625, 0.414215087890625, 0.414215087890625, 0.414215087890625, 0.414215087890625, 0.414215087890625, 0.414215087890625}; 
__declspec(align(32)) const __m256 AVX_tg_3_16 = {-0.331817626953125f, -0.331817626953125f, -0.331817626953125f, -0.331817626953125f, -0.331817626953125f, -0.331817626953125f, -0.331817626953125f, -0.331817626953125f}; 
__declspec(align(32)) const __m256 AVX_tg_3p1_16 = {1.0f - 0.331817626953125f, 1.0f - 0.331817626953125f, 1.0f - 0.331817626953125f, 1.0f - 0.331817626953125f, 1.0f - 0.331817626953125f, 1.0f - 0.331817626953125f, 1.0f - 0.331817626953125f, 1.0f - 0.331817626953125f}; 
__declspec(align(32)) const __m256 AVX_cos_4_16 = {-0.2928924560546875, -0.2928924560546875, -0.2928924560546875, -0.2928924560546875, -0.2928924560546875, -0.2928924560546875, -0.2928924560546875, -0.2928924560546875};
__declspec(align(32)) const __m256 AVX_cos_4p1_16 = {1.0f - 0.2928924560546875, 1.0f - 0.2928924560546875, 1.0f - 0.2928924560546875, 1.0f - 0.2928924560546875, 1.0f - 0.2928924560546875, 1.0f - 0.2928924560546875, 1.0f - 0.2928924560546875, 1.0f - 0.2928924560546875};

__declspec(align(32)) float float_tab_i_04[sizeM8*2];
__declspec(align(32)) float float_tab_i_17[sizeM8*2];
__declspec(align(32)) float float_tab_i_26[sizeM8*2];
__declspec(align(32)) float float_tab_i_35[sizeM8*2];

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
		cout << "idct_AVX Usage" << endl;
		cout << " The performance of a single precision floating point inverse discrete cosine transform is performed using Intel AVX instructions" << endl;
		cout << " Scalar code performs a DCT on randomly initialized data, the IDCT routines" << endl;
		cout << " transform the data. Results are compared to the output of a scalar IDCT function." << endl;
		cout << " The optional command line arguments specifies the block count and iteration count." << endl;
		cout << " Note: Validation against IEEE 1180-1900 requires testing 10000 blocks." << endl;
		cout << " The defaults are block count = " << defaultBlockCount << ", iteration count = " << defaultLoopCount << "." << endl;	
		cout << " idct_AVX blockCount iterationCount" << endl;
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

			inputData[i*blockSize + j] = (float) 0.0f;		
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

	//should be 32 byte alignment for AVX
	inputData = (float *) _mm_malloc(iBufferSizeBytes, AlignmentBytes); 
	kernelResults = (float *) _mm_malloc(iBufferSizeBytes, AlignmentBytes);  
	dctData = (float *) _mm_malloc(iBufferSizeBytes, AlignmentBytes); 
	inverseData = (float *) _mm_malloc(iBufferSizeBytes, AlignmentBytes);  

	//Create tables of constants 
	//Create the transpose of M8 (refer to AP-922 and AP-924)
	float M8_k_values[sizeM8] = {
		4.0f, 2.0f, 4.0f, 6.0f,
		4.0f, 6.0f, 4.0f, 2.0f,
		4.0f, 6.0f, 4.0f, 2.0f,
		4.0f, 2.0f, 4.0f, 6.0f,
		1.0f, 3.0f, 5.0f, 7.0f,
		3.0f, 7.0f, 1.0f, 5.0f,
		5.0f, 1.0f, 7.0f, 3.0f,
		7.0f, 5.0f, 3.0f, 1.0f
	};

	float signsTransposeM8[sizeM8] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, -1.0f,
	};
	float transposeM8[sizeM8];
	for(int i = 0; i < sizeM8; i++)	{
		transposeM8[i] = 0.25f*signsTransposeM8[i]*cosf(M8_k_values[i]*g_pi/16.0f);
	}

	//Create the table of constants that rows 0 and 4 will use.
	//The index_tab_i table specifies the store order in the final tables.
	int index_tab_i3[sizeM8] = {
		0, 4, 8, 12,				//Constants for float 0
		1, 5, 9, 13,				//Constants for float 2
		16, 20, 24, 28,				//Constants for float 1
		17, 21, 25, 29,				//Constants for float 3
		2, 6, 10, 14,				//Constants for float 4
		3, 7, 11, 15,				//Constants for float 6
		18, 22, 26, 30,				//Constants for float 5
		19, 23, 27, 31				//Constants for float 7
	};

	//Need transpose of M8 times cosine(4*pi/16) in the right order
	float value;
	float k = 4.0f;
	for(int i = 0; i < 8; i++)	{
		for(int j = 0; j < 4; j++)	{
			value = transposeM8[index_tab_i3[(i*4) + j]]*cosf(k*g_pi/16.0f);
			
			//Initialize the table used by AVX float implementation
			float_tab_i_04[i*8+j] = value;
			float_tab_i_04[i*8+j+4] = value;					
		}
	}

	//Create the table of constants that rows 1 and 7 will use.
	//Need transpose of M8 times cosine(1*pi/16) in the right order
	k = 1.0f;
	for(int i = 0; i < 8; i++)	{
		for(int j = 0; j < 4; j++)	{
			value = transposeM8[index_tab_i3[(i*4) + j]]*cosf(k*g_pi/16.0f);
			float_tab_i_17[i*8+j] = value;
			float_tab_i_17[i*8+j+4] = value;	
		}
	}

	//Create the table of constants that rows 2 and 6 will use.
	//Need transpose of M8 times cosine(2*pi/16) in the right order
	k = 2.0f;
	for(int i = 0; i < 8; i++)	{
		for(int j = 0; j < 4; j++)	{
			value = transposeM8[index_tab_i3[(i*4) + j]]*cosf(k*g_pi/16.0f);
			float_tab_i_26[i*8+j] = value;
			float_tab_i_26[i*8+j+4] = value;		
		}
	}

	//Create the table of constants that rows 3 and 5 will use.
	//Need transpose of M8 times cosine(3*pi/16) in the right order
	k = 3.0f;
	for(int i = 0; i < 8; i++)	{
		for(int j = 0; j < 4; j++)	{
			value = transposeM8[index_tab_i3[(i*4) + j]]*cosf(k*g_pi/16.0f);
			float_tab_i_35[i*8+j] = value;
			float_tab_i_35[i*8+j+4] = value;	
		}
	}

	return 0;
}

//Memory free
int DeAllocateBuffers()
{
	//free allocated buffers
	_mm_free(inputData);
	_mm_free(kernelResults);
	_mm_free(dctData);
	_mm_free(inverseData);

	return 0;
}

//The AVX single precision floating point IDCT
void idctAVX()
{
	__m128 r_xmm0, r_xmm2, r_xmm1;
	__m256 r_ymm02, r_ymm46, r_ymm13, r_ymm57;
	__m256 r_ymm0, r_ymm1, r_ymm2, r_ymm3, r_ymm4, r_ymm5, r_ymm6, r_ymm7;
	__m256 row0, row1, row2, row3, row4, row5, row6, row7;
	__m256 ymm_even, ymm_odd, ymm_sum, ymm_diff;
	__m256 temp3, temp7;
	__m256 tangent_1, tangent_2, tangent_3, cos_4;

	tangent_1 = AVX_tg_1_16;
	tangent_2 = AVX_tg_2_16;
	tangent_3 = AVX_tg_3p1_16;
	cos_4 = AVX_cos_4p1_16;

	const float * pInput;
	float * pOutput;
	float * pFTab_i_04 = float_tab_i_04;
	float * pFTab_i_26 = float_tab_i_26;
	float * pFTab_i_17 = float_tab_i_17;
	float * pFTab_i_35 = float_tab_i_35;
	const int blockSize = 8*8;

	//Transform all the blocks N times
	//The iteration count is a command line option
	const int maxLoopCount = g_loopCount;
	long startTime = getTimestamp();

	for(int loopCount = 0; loopCount < maxLoopCount; loopCount++)	{
	
		//Operate on all the blocks
		for(int i = 0; i < g_blockCount; i++)	{

			//Get pointers for this input and output
			pInput = &dctData[i*blockSize];				//use random data that is processed by DCT
			pOutput = &kernelResults[i*blockSize];
			pFTab_i_04 = float_tab_i_04;
			pFTab_i_26 = float_tab_i_26;
			pFTab_i_17 = float_tab_i_17;
			pFTab_i_35 = float_tab_i_35;

			//Rows 0 and 4
			//Process the first four floats of these two rows
			//Read input data from row 0, read eight floats via two 128-bit loads
			r_ymm0 = _mm256_castps128_ps256(_mm_load_ps(pInput));
			r_ymm1 = _mm256_castps128_ps256(_mm_load_ps(&pInput[4]));
			r_ymm0 = _mm256_insertf128_ps(r_ymm0, _mm_load_ps(&pInput[4*8]), 1);
			r_ymm4 = _mm256_insertf128_ps(r_ymm1, _mm_load_ps(&pInput[4*8+4]), 1);

			//Broadcast float 0 
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0x00);

			//Multiply by the coefficients
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) pFTab_i_04));

			//Broadcast float 2 and multiply by coefficients
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0xaa);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_04[8]));

			//Add
			r_ymm02 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Broadcast float 1 and multiply by coefficients
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0x55);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_04[16]));

			//Broadcast float 3 and multiply by coefficients
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0xff);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_04[24]));

			//Add
			r_ymm13 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process the second four floats of these two rows
			//Broadcast float 4 
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0x00);

			//Multiply by the coefficients
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_04[32]));

			//Broadcast float 6 and multiply by coefficients
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0xaa);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_04[40]));

			//Add
			r_ymm46 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Broadcast float 5 and multiply by coefficients
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0x55);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_04[48]));

			//Broadcast float 7 and multiply by coefficients
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0xff);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_04[56]));

			//Add to create this partial sum, then create final sums and differences
			r_ymm57 = _mm256_add_ps(r_ymm2, r_ymm3);
			ymm_even = _mm256_add_ps(r_ymm02, r_ymm46);
			ymm_odd = _mm256_add_ps(r_ymm13, r_ymm57);
			ymm_diff = _mm256_sub_ps(ymm_even, ymm_odd);
			ymm_sum = _mm256_add_ps(ymm_even, ymm_odd);

			//Reverse the order of the differences, then build outputs 0 and 4
			ymm_diff = _mm256_shuffle_ps(ymm_diff, ymm_diff, 0x1b);
			row0 = _mm256_permute2f128_ps(ymm_sum, ymm_diff, 0x20);
			row4 = _mm256_permute2f128_ps(ymm_sum, ymm_diff, 0x31);
			//End of Rows 0 and 4 processing

			//Rows 1 and 7
			//Process the first four floats of these two rows
			//Read input data from row 1, read eight floats via two 128-bit loads
			r_ymm0 = _mm256_castps128_ps256(_mm_load_ps(&pInput[8]));
			r_ymm1 = _mm256_castps128_ps256(_mm_load_ps(&pInput[8+4]));
			r_ymm0 = _mm256_insertf128_ps(r_ymm0, _mm_load_ps(&pInput[7*8]), 1);
			r_ymm4 = _mm256_insertf128_ps(r_ymm1, _mm_load_ps(&pInput[7*8+4]), 1);

			//Process floats 0 and 2 
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0x00);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) pFTab_i_17));
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0xaa);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_17[8]));
			r_ymm02 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process floats 1 and 3
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0x55);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_17[16]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0xff);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_17[24]));
			r_ymm13 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process the second four floats of these two rows
			//Process floats 4 and 6
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0x00);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_17[32]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0xaa);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_17[40]));
			r_ymm46 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process floats 5 and 7
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0x55);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_17[48]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0xff);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_17[56]));
			r_ymm57 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Create final sums and differences
			ymm_even = _mm256_add_ps(r_ymm02, r_ymm46);
			ymm_odd = _mm256_add_ps(r_ymm13, r_ymm57);
			ymm_diff = _mm256_sub_ps(ymm_even, ymm_odd);
			ymm_sum = _mm256_add_ps(ymm_even, ymm_odd);

			//Reverse the order of the differences, then build outputs 1 and 7
			ymm_diff = _mm256_shuffle_ps(ymm_diff, ymm_diff, 0x1b);
			row1 = _mm256_permute2f128_ps(ymm_sum, ymm_diff, 0x20);
			row7 = _mm256_permute2f128_ps(ymm_sum, ymm_diff, 0x31);
			//End of Rows 1 and 7 processing

			//Rows 2 and 6
			//Process the first four floats of these two rows
			//Read input data from row 2, read eight floats via two 128-bit loads
			r_ymm0 = _mm256_castps128_ps256(_mm_load_ps(&pInput[2*8]));
			r_ymm1 = _mm256_castps128_ps256(_mm_load_ps(&pInput[2*8+4]));
			r_ymm0 = _mm256_insertf128_ps(r_ymm0, _mm_load_ps(&pInput[6*8]), 1);
			r_ymm4 = _mm256_insertf128_ps(r_ymm1, _mm_load_ps(&pInput[6*8+4]), 1);

			//Process floats 0 and 2 
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0x00);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) pFTab_i_26));
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0xaa);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_26[8]));
			r_ymm02 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process floats 1 and 3
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0x55);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_26[16]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0xff);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_26[24]));
			r_ymm13 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process the second four floats of these two rows
			//Process floats 4 and 6
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0x00);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_26[32]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0xaa);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_26[40]));
			r_ymm46 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process floats 5 and 7
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0x55);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_26[48]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0xff);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_26[56]));
			r_ymm57 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Create the final sum and difference
			ymm_even = _mm256_add_ps(r_ymm02, r_ymm46);
			ymm_odd = _mm256_add_ps(r_ymm13, r_ymm57);
			ymm_diff = _mm256_sub_ps(ymm_even, ymm_odd);
			ymm_sum = _mm256_add_ps(ymm_even, ymm_odd);

			//Reverse the order of the differences, then build outputs 2 and 6
			ymm_diff = _mm256_shuffle_ps(ymm_diff, ymm_diff, 0x1b);
			row2 = _mm256_permute2f128_ps(ymm_sum, ymm_diff, 0x20);
			row6 = _mm256_permute2f128_ps(ymm_sum, ymm_diff, 0x31);
			//End of Rows 2 and 6 processing

			//Rows 3 and 5
			//Process the first four floats of these two rows
			//Read input data from row 2, read eight floats via two 128-bit loads
			r_ymm0 = _mm256_castps128_ps256(_mm_load_ps(&pInput[3*8]));
			r_ymm1 = _mm256_castps128_ps256(_mm_load_ps(&pInput[3*8+4]));
			r_ymm0 = _mm256_insertf128_ps(r_ymm0, _mm_load_ps(&pInput[5*8]), 1);
			r_ymm4 = _mm256_insertf128_ps(r_ymm1, _mm_load_ps(&pInput[5*8+4]), 1);

			//Process floats 0 and 2 
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0x00);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) pFTab_i_35));
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0xaa);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_35[8]));
			r_ymm02 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process floats 1 and 3
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0x55);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_35[16]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm0, r_ymm0, 0xff);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_35[24]));
			r_ymm13 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process the second four floats of these two rows
			//Process floats 4 and 6
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0x00);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_35[32]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0xaa);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_35[40]));
			r_ymm46 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Process floats 5 and 7
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0x55);
			r_ymm2 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_35[48]));
			r_ymm1 = _mm256_shuffle_ps(r_ymm4, r_ymm4, 0xff);
			r_ymm3 = _mm256_mul_ps(r_ymm1, *((__m256 *) &pFTab_i_35[56]));
			r_ymm57 = _mm256_add_ps(r_ymm2, r_ymm3);

			//Create the final sum and difference
			ymm_even = _mm256_add_ps(r_ymm02, r_ymm46);
			ymm_odd = _mm256_add_ps(r_ymm13, r_ymm57);
			ymm_diff = _mm256_sub_ps(ymm_even, ymm_odd);
			ymm_sum = _mm256_add_ps(ymm_even, ymm_odd);

			//Reverse the order of the differences, then build outputs 3 and 5
			ymm_diff = _mm256_shuffle_ps(ymm_diff, ymm_diff, 0x1b);
			row3 = _mm256_permute2f128_ps(ymm_sum, ymm_diff, 0x20);
			row5 = _mm256_permute2f128_ps(ymm_sum, ymm_diff, 0x31);
			//End of Rows 3 and 5 processing

			//******************************
			//perform 1D IDCT on the columns

			//Multiply several rows by the appropriate tangent value
			r_ymm0 = _mm256_mul_ps(row5, tangent_3);	//row5*(tangent3+1)
			r_ymm1 = _mm256_mul_ps(row3, tangent_3);	//row3*(tangent3+1)
			r_ymm4 = _mm256_mul_ps(row7, tangent_1);	//row7*tangent1
			r_ymm5 = _mm256_mul_ps(row1, tangent_1);	//row1*tangent1
			r_ymm0 = _mm256_add_ps(r_ymm0, row3);		//[row5*tangent3 + row5] + row3
			r_ymm2 = _mm256_sub_ps(row5, r_ymm1);		//row5 - [row1*tangent3 + row1]
			r_ymm7 = _mm256_mul_ps(row6, tangent_2);	//row6*tangent2
			r_ymm3 = _mm256_mul_ps(row2, tangent_2);	//row2*tangent2
			r_ymm5 = _mm256_sub_ps(r_ymm5, row7);		//row1*tangent1 - row7
			r_ymm4 = _mm256_add_ps(r_ymm4, row1);		//row7*tangent1 + row1

			//Save intermediate row 7 results, used as an input later
			temp7 = _mm256_add_ps(r_ymm4, r_ymm0);		//[row7*tangent1 + row1] + [row5*tangent3 + row3]

			//Save intermediate row 3 results, used as an input later
			temp3 = _mm256_add_ps(r_ymm5, r_ymm2);		//[row1*tangent1 - row7] + [row5 - [row1*tangent3 + row1]]

			r_ymm4 = _mm256_sub_ps(r_ymm4, r_ymm0);		//[row7*tangent1 + row1] - [row5*tangent3 + row3]
			r_ymm5 = _mm256_sub_ps(r_ymm5, r_ymm2);		//[row1*tangent1 - row7] - [row5 - [row1*tangent3 + row1]]
			r_ymm1 = _mm256_sub_ps(r_ymm4, r_ymm5);		//{[row7*tangent1 + row1] - [row5*tangent3 + row3]} - {[row1*tangent1 - row7] - [row5 - [row1*tangent3 + row1]]}
			r_ymm4 = _mm256_add_ps(r_ymm4, r_ymm5);		//{[row7*tangent1 + row1] - [row5*tangent3 + row3]} + {[row1*tangent1 - row7] - [row5 - [row1*tangent3 + row1]]}
			r_ymm4 = _mm256_mul_ps(r_ymm4, cos_4);		//multiply by cos_4+1

			r_ymm7 = _mm256_add_ps(r_ymm7, row2);		//row6*tangent2 + row2
			r_ymm3 = _mm256_sub_ps(r_ymm3, row6);		//row2*tangent2 - row6
			r_ymm0 = _mm256_mul_ps(r_ymm1, cos_4);		//multiply by cos_4+1
			r_ymm5 = _mm256_add_ps(row0, row4);			//row0 + row4
			r_ymm6 = _mm256_sub_ps(row0, row4);			//row0 - row4

			r_ymm2 = _mm256_sub_ps(r_ymm5, r_ymm7);		//[row0 + row4] - [row6*tangent2 + row2]
			r_ymm5 = _mm256_add_ps(r_ymm5, r_ymm7);		//[row0 + row4] + [row6*tangent2 + row2]

			r_ymm1 = _mm256_sub_ps(r_ymm6, r_ymm3);		//[row0 - row4] - [row2*tangent2 - row6]
			r_ymm6 = _mm256_add_ps(r_ymm6, r_ymm3);		//[row0 - row4] + [row2*tangent2 - row6]

			r_ymm7 = _mm256_add_ps(temp7, r_ymm5);		//[[row7*tangent1 + row1] + [row5*tangent3 + row3]] + [[row0 + row4] + [row6*tangent2 + row2]]
			
			//Store row 0 results (store 1 of 8)
			_mm256_store_ps(pOutput, r_ymm7);

			r_ymm3 = _mm256_sub_ps(r_ymm6, r_ymm4);		//[[row0 - row4] + [row2*tangent2 - row6]] - cos4*{[row7*tangent1 + row1] - [row5*tangent3 + row3]} + {[row1*tangent1 - row7] - [row5 - [row1*tangent3 + row1]]}
			r_ymm6 = _mm256_add_ps(r_ymm6, r_ymm4);		//[[row0 - row4] + [row2*tangent2 - row6]] + cos4*{[row7*tangent1 + row1] - [row5*tangent3 + row3]} + {[row1*tangent1 - row7] - [row5 - [row1*tangent3 + row1]]}

			//Store row 1 results (store 2 of 8)
			_mm256_store_ps(&pOutput[1*8], r_ymm6); 

			r_ymm7 = _mm256_sub_ps(r_ymm1, r_ymm0);
			r_ymm1 = _mm256_add_ps(r_ymm1, r_ymm0);

			r_ymm6 = _mm256_add_ps(r_ymm2, temp3);
			r_ymm2 = _mm256_sub_ps(r_ymm2, temp3);	 

			r_ymm5 = _mm256_sub_ps(r_ymm5, temp7); 

			//Store final results
			_mm256_store_ps(&pOutput[2*8], r_ymm1); 
			_mm256_store_ps(&pOutput[3*8], r_ymm6); 
			_mm256_store_ps(&pOutput[4*8], r_ymm2);
			_mm256_store_ps(&pOutput[5*8], r_ymm7);
			_mm256_store_ps(&pOutput[6*8], r_ymm3);
			_mm256_store_ps(&pOutput[7*8], r_ymm5); 
		}	
	}

	long duration = getTimestamp() - startTime;
	cout << "AVX Timestamp = " << duration << endl;

	return;
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

	cout.precision(10);
	cout.setf(ios_base::showpoint);

	//Initialize the mean square error accumulators
	for(int i = 0; i < blockSize; i++)	{
		pixelError[i] = 0;
		pixelErrorSquared[i] = 0;
	}

	for(int i = 0; i < g_blockCount; i++)	{
		for(int j = 0; j < blockSize; j++)	{
			double fKernelValue = kernelResults[i*blockSize + j];
			double fReferenceValue = inverseData[i*blockSize + j];
			
			//IEEE 1180-1900 says to round and clip before comparing
			int referenceValue = (int) floor(fReferenceValue + 0.5);
			int kernelValue = (int) floor(fKernelValue + 0.5);
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

			//Check for errors
			if(kernelValue != referenceValue)	{

				//Note the error
				errorCount++;
				errorAccumulator += kernelValue - referenceValue;
				result = false;
				if(errorCount < maxErrors)	{

					//Report the error
					cout << "SIMD float mismatch at position " << j << " of block " << i << ": input = " << referenceValue << ", kernel result = " << kernelValue << endl;
				}

				//Check that magnitude of the error is not greater than 1
				if((abs(kernelValue - referenceValue) > 1) && (errorCount < maxErrors))	{
					
					cout << "SIMD float error is greater than 1" << endl;
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
		ValidateIEEE(errorAccumulator, pixelError, pixelErrorSquared, "SIMD float");

		if(maxErrors < errorCount)	{
			cout << "SIMD float mismatch reporting stopped at " << maxErrors << ", there were " << errorCount << " mismatches." << endl;
		}
	}

	return result;
}

//the function performs a 2D discrete cosine transform
void discreteCosineTransform()
{
	const int size = 8;
	const int blockSize = 8*8;
	const double pi = 3.141592654;//22.0f/7.0f;

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

	//Run the AVX single precision floating point IDCT
	idctAVX();

	if(ValidateResult() == true)	{
		cout << "ValidateResult passed for 256 bits, " << pMessage << endl;
	}

	return true;
}

int main (int argc, char * const argv[]) {
    
    cout << "Welcome to IDCT using AVX" << endl;

	SetKernelParameters(argc, (char * *) argv);
	AllocateAndInitBuffers(32);

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




