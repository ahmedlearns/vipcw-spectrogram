/*

    Convert a Stereo file into a Mono wave file by concatenating the left channel data together.
    Passes determined values from wav file into FFT function.
    Normalizes values based on the range, outputted values are from 0-1.
    Writes output to text file for use with the spectrogram display code.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fft.h"


int main()
{
    int i=0,j,k,l=0;
    int N = 2;                    /* number of points in FFT */
    double (*X)[2];          		 /* pointer to frequency-domain samples */   /* double */
    double x[N][2];             /* double */
    double mag, min, max, mag_norm;                 /* double */
    X = malloc(2 * N * sizeof(double));  /* double */
    while (i<N) {
        x[i][1]=0;
        i++;
    }

    int lSize;
    FILE * f = fopen("wavs/sin.wav", "r"); //opening the 2 channels wave file
    FILE * f1 = fopen ("sinMono.wav", "wb"); // this is  the file where the one channel sound file will be saved
    FILE * f2 = fopen ("sinFFT.txt", "w"); // a text file that will have the left channel sound data,
    //just to see what we're dealing with
    //FILE * f3 = fopen ("pianoSample.csv", "w");  // an excel file that have the data to use in matLab
    fseek (f , 0 , SEEK_END);
    lSize = ftell (f);
    rewind (f);

    typedef struct wave {
        int chunkID;
        int chunkSize;
        int format;
        int subChunk1ID;
        int subChunk1size;
        int audio_num_ch;
        int sampleRate;
        int byteRate;
        int bckAli_bitPs;
        int subChunk2ID;
        int subChunk2size;
        int  data[(lSize-40)/4];
    } wave;
    wave * mywave;
    mywave=(wave *) malloc( lSize);

    fread(mywave,1,lSize,f);


    i=0,j=0;
    k=0;
    int mask=65535;
    int  leftVa;
    while (i<((lSize-40)/4)) {

        leftVa=(mywave->data[i+1]<<16) | (mywave->data[i] & mask);
        mywave->data[j]=leftVa;

        x[k][0]=leftVa;
        j++;
        k++;
        printf("k is %d and N is %d\n", k, N);
        if (k==N) {
            fft(N, x, X);
            min = 0;
            max = sqrt((X[0][0]*X[0][0])+(X[0][1]*X[0][1]));
            for(l=0; l<N; l++) {
                mag=sqrt((X[l][0]*X[l][0])+(X[l][1]*X[l][1]));
                if(mag > max) {
                    max = mag;
                }
                if(mag < min) {
                    min = mag;
                }
            }


            for(l=0; l<N; l++) {
                mag=sqrt((X[l][0]*X[l][0])+(X[l][1]*X[l][1]));

                mag_norm = (mag - min)/(max - min);

                fprintf(f2,"%12f,\n", mag_norm);
            }
            k=0;
        }
        i=i+2;
    }

    mywave->audio_num_ch=mywave->audio_num_ch-65536;
    mywave->chunkSize=(mywave->chunkSize/2)+18;
    mywave->byteRate=mywave->byteRate/2;
    mywave->bckAli_bitPs=mywave->bckAli_bitPs-2;
    mywave->subChunk2size=mywave->chunkSize-36;


    fwrite(mywave,1,(lSize/2)+22,f1);
    fclose (f);
    fclose (f1);
    fclose (f2);
    // fclose (f3);
    free(X);
    return 0;
}
