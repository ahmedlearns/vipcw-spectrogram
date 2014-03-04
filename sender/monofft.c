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
#include "monofft.h"    

/********************** Global Variable ***************************************/
float hamWindow_Multiplier;
static double old_an[257][2];
double prev_avg = 0.0;


void fftw3 (int N, double* output, double target, double weight, char agc_off, double (*x)[2]) //double (*X)[2]) //, double old_an[N/2+1][2])
{
    int i;
    double *in;
    double *in2;
    int nc = ( N / 2 ) + 1;
    fftw_complex *out;
    fftw_plan plan_backward;
    fftw_plan plan_forward;

    struct agc_params* params;
    params = (struct agc_params*) malloc(sizeof(struct agc_params));
    params->nc = nc;
    params->target = target; 
    params->weight = weight;

    /*
        Set up an array to hold the data, and assign the data.
    */
    in = fftw_malloc ( sizeof ( double ) * N );

    // saving new data into input array to compute fft 
    for ( i = 0; i < N; i++ ){
        in[i] = x[i][0];
        hamWindow_Multiplier = 0.5*(1-cos(2*3.1415*i/N));        
        in[i] = hamWindow_Multiplier*in[i];                // Apply hamming window to input (IIR Filter)
    }
        

    /*
        Set up an array to hold the transformed data,
        get a "plan", and execute the plan to transform the IN data to
        the OUT FFT coefficients.
    */
    out = fftw_malloc ( sizeof ( fftw_complex ) * nc );
    plan_forward = fftw_plan_dft_r2c_1d ( N, in, out, FFTW_ESTIMATE );
    fftw_execute ( plan_forward );

    /* AUTOMATIC GAIN COMPUTATION */
    if(!agc_off){
        agc(*params, out, output);
    } else {
        for(i=0; i < params->nc; i++) {
            output[i] = sqrt((out[i][0]*out[i][0])+(out[i][1]*out[i][1]));
        }
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

void agc(struct agc_params params, fftw_complex* out, double* output)
{
    // printf("agc_params=> weight: %f\ttarget:%f\n", params.weight, params.target);
    double avg, scale;
    double scaled_target;
    double mag, min, max, mag_norm;
    int i, imax;

    /**
    * When the output of the fft is normalized before the scale is applied, the scale turns out to be very large, the mag_norm always very small (around .001-.002), and 
    *  so the output comes out to always be 0.75. (normalization then scale)
    *
    * When the the scale was applied before the normalization. The scale would be found using the magnitude of each FFT point, and would the output would then be
    *  the scaled version of both the real and imaginary version. Before the output is sent to the display, it is first normalized. (scale the real and imag then normalize)
    */
    max = 0; 
    imax = -1;

    for(i=0; i < params.nc; i++) {
        mag = sqrt((out[i][0]*out[i][0]) + (out[i][1]*out[i][1]));
        // printf("mag[%d] is %.3f\n", i, mag);
        if(mag > max) {
            max = mag;
            imax = i;
        }
    }

    scaled_target = params.target * max;

    avg = (double) (params.weight * prev_avg + (1.0 - params.weight) * max);
    printf("avg, %.3f, prev_avg, %.3f, max, %.3f, imax, %d, ", avg, prev_avg, max, imax);
    prev_avg = avg;

    scale = scaled_target/avg;
    printf("scale, %.3f\n", scale);

    for(i=0; i < params.nc; i++) {
        mag=sqrt((out[i][0]*out[i][0])+(out[i][1]*out[i][1]));
        output[i] = mag * scale;
    }
}

void genfft(int N, double* in, double* out, double target, double weight, char agc_off)
{
    // printf("IN: monofft:genFFT() \n");

    int i;
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

    for(i=0; i < N; i++) {
        // mag=sqrt((X[i][0]*X[i][0])+(X[i][1]*X[i][1]));
        mag = X[i];
        if(mag > max) 
            max = mag;
        if(mag < min) 
            min = mag;
    }
    // printf("\t NO SEG FAULT YET\n");
    
    for(i=0; i<N; i++) {
        // mag=sqrt((X[i][0]*X[i][0])+(X[i][1]*X[i][1]));
        mag = X[i];
        mag_norm = (mag - min)/(max - min);
        out[i] = mag_norm;
        // printf("out[%d] = %.3f\t", i, out[i]);
        // if(i%4 == 0) printf("\n");
    }

    // free(X);
    return;
}
