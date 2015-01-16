#ifndef __COMPLEX_HPP__
#define __COMPLEX_HPP__

#define PI 3.1415926535897931

typedef struct {
   float real;
   float imag;
} Complex;

void fftSinCos(float x, float* s, float* c);
float abs(const Complex* x);
float arg(const Complex* x);

#endif