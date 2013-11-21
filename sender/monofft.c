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
#include "fft.h"
#include "fft_sender.h" 

void genfft(int N, double* in, double** out)
{
    // printf("IN: monofft:genFFT(): sizeof(cb->fbuffer)=%d\n", sizeof(cb->fbuffer));

    int i;
    double (*X)[2];                  /* pointer to frequency-domain samples */   
    double x[N][2];             
    double mag, min, max, mag_norm;                 
    X = (double*) malloc(N * sizeof(double)); // previously: malloc(2 * N * sizeof(double)) 

    for(i = 0; i < N; i++){
        x[i][0] = in[i];
        x[i][1] = 0;
    }
    
    fft(N, x, X);
    min = 0;
    max = sqrt( (X[0][0] * X[0][0]) + (X[0][1] * X[0][1]) );

    for(i=0; i<N; i++) {
        mag=sqrt((X[i][0]*X[i][0])+(X[i][1]*X[i][1]));
        if(mag > max) 
            max = mag;
        if(mag < min) 
            min = mag;
    }
    
    for(i=0; i<N; i++) {
        mag=sqrt((X[i][0]*X[i][0])+(X[i][1]*X[i][1]));
        mag_norm = (mag - min)/(max - min);
        (*out)[i] = mag_norm;
    }

    free(X);
    return;
}
