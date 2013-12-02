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
static double old_an[129][2];


void fftw3 ( int N, double (*x)[2], double (*X)[2], double old_an[N/2+1][2])
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
for ( i = 0; i < nc; i++ )
  {
   newMag= (double)sqrt(((old_an[i][0])*(old_an[i][0])) + ((old_an[i][1])*(old_an[i][1])));
    if(mag<newMag)
        mag=newMag;        // new maximum magnitude
   }

double maxFFT= mag;
double scale = 1/maxFFT;        // scaleFactorNew =1/maxFFTofnew 
double weightFactor = 0.9;

/* Saving fftw3 output coefficient into X and old_an */
 for ( i = 0; i < nc; i++ )
  {
     old_an[i][0] = (double)((weightFactor) * out[i][0]  + ((1-weightFactor)*scale));
     old_an[i][1] = (double)((weightFactor) * out[i][1]  + ((1-weightFactor)*scale));
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


void genfft(int N, double* in, double* out)
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
    
    fftw3(N, x, X, old_an);
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
static double[129][2] 

/********************** Global Variable ***************************************/
float hamWindow_Multiplier;

/*****************************new added***************************************/
void fftw ( int N, double (*x)[2], double (*X)[2])

/******************************************************************************/
/*
  Purpose:

    TEST02: apply FFT to real 1D data.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    23 October 2005

  Author:

    John Burkardt
*/
{
  int i;
  double *in;
  double *in2;
  //int n = 100;
  int nc;
  fftw_complex *out;
  fftw_plan plan_backward;
  fftw_plan plan_forward;
  unsigned int seed = 123456789;

/*
  Set up an array to hold the data, and assign the data.
*/
  in = fftw_malloc ( sizeof ( double ) * N );

  
  for ( i = 0; i < N; i++ )
  {
    in[i] = x[i][0];
    
    hamWindow_Multiplier = 0.5*(1-cos(2*3.1415*i/N));   // Multiplying with hamming window
    
    in[i] = hamWindow_Multiplier*in[i];
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



/*
double mag=0.0;
double newMag=0.0;
for ( i = 0; i < nc; i++ )
  {
   newMag= (double)sqrt(((old_an[i][0])*(old_an[i][0])) + ((old_an[i][1])*(old_an[i][1])));
    if(mag<newMag)
    mag=newMag;
   }
//double maxFFT= maxFFTout(&out, nc);   //computes maximum of magnitude of FFT output values
double maxFFT= mag;
double scale = 1/maxFFT;            // scaleFactorNew =1/maxFFTofnew 
double weightFactor = 0.9;
float real, imag;
 for ( i = 0; i < nc; i++ )
  {
     out[i][0] = (double)((weightFactor) * out[i][0]  + ((1-weightFactor)*scale));
     out[i][1] = (double)((weightFactor) * out[i][1]  + ((1-weightFactor)*scale));
    old_an[i][0]=(double)out[i][0];
    old_an[i][1]=(double)out[i][1];
  }
*/





  
  /*printf ( "\n" );
  printf ( "  Output FFT Coefficients:\n" );
  printf ( "\n" );
 */


  for ( i = 0; i < nc; i++ )    // Saving fftw3 output to X
  {
    X[i][0] = out[i][0];    
    X[i][1] = out[i][1];
  }

/**********************Don't need to recover input*******************

  Set up an arrray to hold the backtransformed data IN2,
  get a "plan", and execute the plan to backtransform the OUT
  FFT coefficients to IN2.

  in2 = fftw_malloc ( sizeof ( double ) * n );

  plan_backward = fftw_plan_dft_c2r_1d ( n, out, in2, FFTW_ESTIMATE );

  fftw_execute ( plan_backward );

  printf ( "\n" );
  printf ( "  Recovered input data divided by N:\n" );
  printf ( "\n" );

  for ( i = 0; i < n; i++ )
  {
    printf ( "  %4d  %12f\n", i, in2[i] / ( double ) ( n ) );
  }
*/


/*
  Release the memory associated with the plans.
*/
  fftw_destroy_plan ( plan_forward );
  //fftw_destroy_plan ( plan_backward );

  fftw_free ( in );
  //fftw_free ( in2 );
  fftw_free ( out );

  return;
}
/******************************************************************************/
