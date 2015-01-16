#include "fann.h"
#include "parroto.h"
extern ParrotObserver parroto;


#include "fourier.hpp"
#include <cmath>
#include <fstream>
#include <map>

void calcFftIndices(int K, int* indices)
{
	int i, j ;
	int N ;

	N = (int)log2f(K) ;

	indices[0] = 0 ;
	indices[1 << 0] = 1 << (N - (0 + 1)) ;
	for (i = 1; i < N; ++i)
	{
		for(j = (1 << i) ; j < (1 << (i + 1)); ++j)
		{
			indices[j] = indices[j - (1 << i)] + (1 << (N - (i + 1))) ;
		}
	}
}

void radix2DitCooleyTykeyFft(int K, int* indices, Complex* x, Complex* f)
{

	calcFftIndices(K, indices) ;

	int step ;
	float arg ;
	int eI ;
	int oI ;

	float fftSin;
	float fftCos;

	Complex t;
	int i ;
	int N ;
	int j ;
	int k ;

	double dataIn[1];
	double dataOut[2];

	for(i = 0, N = 1 << (i + 1); N <= K ; i++, N = 1 << (i + 1))
	{
		for(j = 0 ; j < K ; j += N)
		{
			step = N >> 1 ;
			for(k = 0; k < step ; k++)
			{
				arg = (float)k / N ;
				eI = j + k ;
				oI = j + step + k ;

				dataIn[0] = arg;

// #pragma parrot(input, "fft", [1]dataIn)
	struct fann *ann;
	fann_type *parrotOut;
	fann_type parrotIn[1];
	parrotIn[0] = dataIn[0];
	ann = fann_create_from_file("fann.config/fft.nn");
	parrotOut = fann_run(ann, (fann_type*)parrotIn);
// parroto.write("fft", 0, dataIn, 1, 0, 0);
// 
// 				fftSinCos(arg, &fftSin, &fftCos);
// 
// 				dataOut[0] = fftSin;
// 				dataOut[1] = fftCos;
// 
// parroto.write("fft", 1, dataOut, 2, 0.0, 2.0);
// #pragma parrot(output, "fft", [2]<0.0; 2.0>dataOut)
	dataOut[0] = parrotOut[0];
	dataOut[1] = parrotOut[1];

				fftSin = dataOut[0];
				fftCos = dataOut[1];


				// Non-approximate
				t =  x[indices[eI]] ;
				x[indices[eI]].real = t.real + (x[indices[oI]].real * fftCos - x[indices[oI]].imag * fftSin);
                x[indices[eI]].imag = t.imag + (x[indices[oI]].imag * fftCos + x[indices[oI]].real * fftSin);

                x[indices[oI]].real = t.real - (x[indices[oI]].real * fftCos - x[indices[oI]].imag * fftSin);
                x[indices[oI]].imag = t.imag - (x[indices[oI]].imag * fftCos + x[indices[oI]].real * fftSin);
			}
		}
	}

	for (int i = 0 ; i < K ; i++)
	{
		f[i] = x[indices[i]] ;
	}
}
