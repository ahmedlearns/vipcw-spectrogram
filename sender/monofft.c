/*

    Convert a Stereo file into a Mono wave file by concatenating the left channel data together.
    Passes determined values from wav file into FFT function.
    Normalizes values based on the range, outputted values are from 0-1.
    Writes output to text file for use with the spectrogram display code.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
    
#include <fftw3.h>
// #include "fft.h"

#include "fft_sender.h" 
// #include "fftw3.h"

/********************** Global Variable ***************************************/
float hamWindow_Multiplier;
static double old_an[257][2];


void fftw3 ( int N, double (*x)[2], double (*X)[2], double t, double w) //, double old_an[N/2+1][2])
{
  int i;
  double *in;
  double *in2;
  //int n = 100;
  int nc;
  fftw_complex *out;
  fftw_plan plan_backward;
  fftw_plan plan_forward;

/*
  Set up an array to hold the data, and assign the data.
*/
  in = fftw_malloc ( sizeof ( double ) * N );

  // saving new data into input array to compute fft 
  for ( i = 0; i < N; i++ )
  {
    in[i] = x[i][0];
        
    hamWindow_Multiplier = 0.5*(1-cos(2*3.1415*i/N));        
    
    in[i] = hamWindow_Multiplier*in[i];                // Apply hamming window to input (IIR Filter)
  }
        

/*
  Set up an array to hold the transformed data,
  get a "plan", and execute the plan to transform the IN data to
  the OUT FFT coefficients.
*/
  nc = ( N / 2 ) + 1;

  out = fftw_malloc ( sizeof ( fftw_complex ) * nc );

  plan_forward = fftw_plan_dft_r2c_1d ( N, in, out, FFTW_ESTIMATE );

  fftw_execute ( plan_forward );

/* AUTOMATIC GAIN COMPUTATION */
double mag=0.0;
double newMag=0.0;

// After this loop below, maximum magnitude of previous data set is found
/*
for ( i = 0; i < nc; i++ )
  {
   newMag= (double)sqrt(((old_an[i][0])*(old_an[i][0])) + ((old_an[i][1])*(old_an[i][1])));
    if(mag<newMag)
        mag=newMag;        // new maximum magnitude
   }
*/
double maxFFT= mag;
double scale = 1/maxFFT;        // scaleFactorNew =1/maxFFTofnew 
double weightFactor = 0.9;

/* Saving fftw3 output coefficient into X and old_an */
 for ( i = 0; i < nc; i++ )
  {
     // old_an[i][0] = (double)((weightFactor) * out[i][0]  + ((1-weightFactor)*scale));
     // old_an[i][1] = (double)((weightFactor) * out[i][1]  + ((1-weightFactor)*scale));
     X[i][0] = out[i][0];        
     X[i][1] = out[i][1];
  }


/*
  Release the memory associated with the plans.
*/
  fftw_destroy_plan ( plan_forward );
  fftw_free ( in );
  fftw_free ( out );

  return;
}
/******************************************************************************/


void genfft(int N, double* in, double* out, double t, double w)
{
    // printf("IN: monofft:genFFT() \n");

    int i;
    double (*X)[2];                  /* pointer to frequency-domain samples */   
    double x[N][2];             
    double mag, min, max, mag_norm;                 
    X = (double*) malloc(2 * N * sizeof(double));

    for(i = 0; i < N; i++){
        x[i][0] = in[i];
        x[i][1] = 0;
    }
    
    fftw3(N, x, X, t, w); // , old_an);
	// fft(N, x, X);
    min = 0;
    max = sqrt( (X[0][0] * X[0][0]) + (X[0][1] * X[0][1]) );

    for(i=0; i < N; i++) {
        mag=sqrt((X[i][0]*X[i][0])+(X[i][1]*X[i][1]));
        if(mag > max) 
            max = mag;
        if(mag < min) 
            min = mag;
    }
    // printf("\t NO SEG FAULT YET\n");
    
    for(i=0; i<N; i++) {
        mag=sqrt((X[i][0]*X[i][0])+(X[i][1]*X[i][1]));
        mag_norm = (mag - min)/(max - min);
        out[i] = mag_norm;
    }

    free(X);
    return;
}
