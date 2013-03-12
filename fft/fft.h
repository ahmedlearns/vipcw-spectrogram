/*
 * fft.h
 *
 *  Created on: Mar 16, 2011
 *      Author: masyllis
 */
#ifndef FFT_H_
#define FFT_H_

/* function prototypes */
void fft(int N, double (*x)[2], double (*X)[2]);   /* originally set as doubles for the arrays */
void fft_rec(int N, int offset, int delta,
             double (*x)[2], double (*X)[2], double (*XX)[2]);        /* originally set as doubles for the arrays */
void ifft(int N, double (*x)[2], double (*X)[2]);                     /* originally set as doubles for the arrays */
//void monofft(char *filename);

#endif /* FFT_H_ */
