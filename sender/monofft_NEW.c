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

#include "fft_sender.h" 

/********************** Global Variable ***************************************/
float hamWindow_Multiplier;
// static double old_an[257][2];
static double prev_avg = 0.0;


void fftw3 (int N, double* output, double target, double weight, char agc_off, double (*x)[2]){
    int i;
    double *in;
    double *in2;
    //int n = 100;
    int nc;
    fftw_complex *out;
    fftw_plan plan_backward;
    fftw_plan plan_forward;
    double mag;

    /*
    Set up an array to hold the data, and assign the data.
    */
    in = fftw_malloc ( sizeof ( double ) * N );

    // saving new data into input array to compute fft 
    for ( i = 0; i < N; i++ ){
    in[i] = x[i][0];
    hamWindow_Multiplier = 0.5 * (1 - cos(2 * 3.1415 * i/N));        
    in[i] = hamWindow_Multiplier * in[i];                // Apply hamming window to input (IIR Filter)
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

    /* Automatic Gain Computation */
    // printf("agc_off is %d\n", agc_off);
    if(!agc_off)
        agc(target, weight, out, output);

    for(i=0; i < nc; i++) {
        mag=sqrt((out[i][0]*out[i][0])+(out[i][1]*out[i][1]));
        output[i] = mag;
        // printf("output[%d] is %.3f\n", i, mag);
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

void agc(double target, double weight, fftw_complex* out, double* output){
    /* AUTOMATIC GAIN COMPUTATION */

    double avg, scale;
    // double prev_avg;// = 0.75;
    double scaled_target;// = 0.75;
    // double weight = 0.99;
    double mag, min, max, mag_norm;
    int imax, i, nc;

    /**
    * When the output of the fft is normalized before the scale is applied, the scale turns out to be very large, the mag_norm always very small (around .001-.002), and 
    *  so the output comes out to always be 0.75. (normalization then scale)
    *
    * When the the scale was applied before the normalization. The scale would be found using the magnitude of each FFT point, and would the output would then be
    *  the scaled version of both the real and imaginary version. Before the output is sent to the display, it is first normalized. (scale the real and imag then normalize)
    */

    /* First normalize the output */
    // min = 0;
    max = 0; //sqrt( (out[0][0] * out[0][0]) + (out[0][1] * out[0][1]) );
    imax = -1;

    for(i=0; i < nc; i++) {
        mag=sqrt((out[i][0]*out[i][0])+(out[i][1]*out[i][1]));
        printf("mag[%d] is %.3f\n", i, mag);
        if(mag > max) {
            max = mag;
            imax = i;
      }
      //~ if(mag < min) 
          //~ min = mag;
    }
    // printf("\t NO SEG FAULT YET\n");

    scaled_target = target * max;
    // prev_avg = 0.0;

    avg = (double) (weight * prev_avg + (1.0 - weight) * max);
    // printf("avg: %.3f\t prev_avg: %.3f\t max: %.3f\n", avg, prev_avg, max);
    printf("avg, %.3f, prev_avg, %.3f, max, %.3f, imax, %d, ", avg, prev_avg, max, imax);
    prev_avg = avg;

    scale = scaled_target/avg;
    printf("scale, %.3f\n", scale);

    for(i=0; i < nc; i++) {
        mag=sqrt((out[i][0]*out[i][0])+(out[i][1]*out[i][1]));
        // mag_norm = (mag - min)/(max - min);


        output[i] = mag * scale;        
        // output[i][1] = out[i] * scale;
        // printf("output[%d], %.3f\n", i, output[i]);

        // output[i] = mag_norm;
        // printf("out[%d] = %.3f\t", i, out[i]);
        // if(i%4 == 0) printf("\n");
    }

/* Saving fftw3 output coefficient into X, scaled using AGC */
// for ( i = 0; i < nc; i++ ){
//     // mag = (double)sqrt(((out[i][0])*(out[i][0])) + ((out[i][1])*(out[i][1])));
    
//     // if(mag < newMag)
//     //     mag = newMag;        // new maximum magnitude
// }

}

void genfft(int N, double* in, double* out, double target, double weight, char agc_off)
{
    // printf("IN: monofft:genFFT() \n");

    int i, nc = N/2 + 1;
    // double (*X)[2];                  /* pointer to frequency-domain samples */   
    double x[N][2];
    double X[N/2 + 1];            
    double mag, min, max, mag_norm;                 
    // X = (double*) malloc(2 * N * sizeof(double));

    /* initialize input for FFT */
    for(i = 0; i < N; i++){
        x[i][0] = in[i];
        x[i][1] = 0;
    }
    
    fftw3(N, X, target, weight, agc_off, x); // , old_an);
	// fft(N, x, X);
    // min = 0;
    // max = sqrt( (X[0][0] * X[0][0]) + (X[0][1] * X[0][1]) );
    int imax = -1;
    max = X[0];
    for(i=0; i < nc; i++) {
        // mag=sqrt((X[i][0]*X[i][0])+(X[i][1]*X[i][1]));
        mag = X[i];
        if(mag > max){ 
            max = mag;
            imax = 1;
        }
        if(mag < min) 
            min = mag;
    }
    printf("max mag is at index %d\n", imax);
    // printf("\t NO SEG FAULT YET\n");
    
    for(i=0; i<nc; i++) {
        // mag=sqrt((X[i][0]*X[i][0])+(X[i][1]*X[i][1]));
        mag = X[i];
        mag_norm = (mag - min)/(max - min);
        out[i] = mag_norm;
        // printf("out[%d] = %.3f\n ", i, out[i]);
        // if(i%4 == 0) printf("\n");
    }

    // free(X);
    return;
}
