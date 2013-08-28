/**
 * monofft.h
 */

#include <fftw3.h>

void genfft(float*, int, fftw_plan p, fftw_complex *in, fftw_complex *out);