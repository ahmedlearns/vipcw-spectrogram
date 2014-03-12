/* fft_sender.c
 *
 * This file will contain functions that will package fft data to be sent 
 * through a socket.
 *
 * 10/29/2013   Separate CircularBuffer functions to external file.
 * 
 * Author: Ahmed Ismail
 */

#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#include "fft_sender.h"
#include "fft_socket_header.h"
#include "monofft.h"

#define WAV_HEADER_SIZE 48  

int debug = 1;

fft_header* hdr; 

void err(const char *msg)
{
    if(debug)fprintf(stderr, "%s\n", msg);
    exit(0);
}

void print_help_and_exit(void) {
    printf("  -h\t\t\tThis helpful output\n");
    exit(0);
}

int main(int argc, char *argv[])
{

    int opt;
    int r = DEFAULT_R;
    int s = DEFAULT_S;
    int f = DEFAULT_F;
    double t = DEFAULT_T;
    double w = DEFAULT_W;
    char a = DEFAULT_A;
    char c = DEFAULT_C;
    int n;

    /* Read arguments */ 
    while(-1 != (opt = getopt(argc, argv, "r:s:f:a:t:w:n:c:h"))) {
        switch(opt) {
            case 'r':
                r = atoi(optarg);
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'f':
                f = atoi(optarg);
                break;
            case 'a':
                a = atoi(optarg);
                break;
            case 'c':
                c = atoi(optarg);
                break;
            case 't':
                t = atof(optarg);
                break;
            case 'w':
                w = atof(optarg);
                break;
            case 'n':
                n = atoi(optarg);
                break;
            case 'h':
            /* Fall through */
            default:
                print_help_and_exit();
                break;
        }
    }

    if(debug)
        printf("IN: fft_sender:main()\n");

    if(debug) printf("IN: fft_sender:main()\n");
    printf("Source_Sender Settings\n");
    printf("r: %d\n", r);
    printf("s: %d\n", f);
    printf("f: %d\n", f);
    printf("n: %d\n", n);
    printf("a: %d\n", a);
    printf("t: %.3f\n", t);
    printf("w: %.3f\n", w);
    printf("\n");

    if(!write_audio(n, s, f, t, w, a, c));
        if(debug)fprintf(stderr, "ERROR, write failed\n");
    return 0;
}

// void updateTime(struct timeval timestamp){
    // gettimeofday(hdr->timestamp, NULL);
// }

//void init_fft(int bytesToNextHeader, int samplesToNextFFT, int ptsPerFFT, 
//      struct timeval timestamp, int sampFreq)
void init_fft(int bytesToNextHeader, int samplesToNextFFT, int ptsPerFFT, 
             int sampFreq, int fftRate, int endTrans)
{
    if(debug) printf("IN: fft_sender:init_fft()\n");
    hdr = (fft_header*) malloc(sizeof(fft_header));
    hdr->constSync = 0xACFDFFBC;
	hdr->ptsPerFFT = ptsPerFFT/2 + 1;
    hdr->bytesToNextHeader = sizeof(double) * (ptsPerFFT/2+1) + sizeof(fft_header);
    hdr->samplesToNextFFT = samplesToNextFFT;
    //updateTime(timestamp);
    hdr->sampleRate = sampFreq;
    // -1 when ongoing, 1 to signal end of transmission
    hdr->fftRate = fftRate;
    hdr->endTrans = endTrans;

    // RETURN int ERROR CODES
}

/** 
 * Empty out the stdin buffer to the latest N samples.
 * 
 *  Assuming samples are 16 bits long. Each sample contains both channels: 
 *      (LEFT|RIGHT)(LEFT|RIGHT)
 *      \  16 bits /\  16 bits /
 *
 *  Seperate channels by only taking the left channel. The sample data is a 2's complement signed integer. 
 *      Use short signed int (or int2). See fft_sender.h
 *              OR
 *      Use char's to only read left channel data.
 *
 */
void get_samples(int N, double* dbuffer, int fftRate, char channel)
{

    if(debug) printf("IN: fft_sender:get_samples(%d)\n", N);
    
    static int init_samples = 0;
    if(!init_samples)
    {
        sleep(2);

        char wav_header[48];
        // wave* wav_header = (wave*) malloc(sizeof(wave));

        // int n = fgets(wav_header, WAV_HEADER_SIZE, stdin);
        int n = read(fileno(stdin), wav_header, WAV_HEADER_SIZE);
        /* Read in Wave header to set fft_header and then discard*/
        // if(WAV_HEADER_SIZE != read(fileno(stdin), wav_header, WAV_HEADER_SIZE)){
        if(WAV_HEADER_SIZE != n){
            //if(debug) printf("Read %d bytes into wav_header\n", n);
            err("Error discarding Wave header");
        }
        int ptsPerFFT = N;         // number of points per FFT 
        int bytesToNextHeader = ptsPerFFT * sizeof(double) + sizeof(fft_header);  // total amount of space (header+data)
        // int sampleRate = wav_header->sampleRate;
        int sampleRate = 22050;
        // int fftRate = 10;       // 10 FFT's per second.
        int samplesToNextFFT = (sampleRate / fftRate) - ptsPerFFT;   // Num samples to the start of the next FFT
        int endTrans = 5;

        init_fft(bytesToNextHeader, samplesToNextFFT, ptsPerFFT, sampleRate, fftRate, endTrans);
        	if(debug) printf("\t Back in get_samples() from init_fft()\n");
        // sleep(1);
        init_samples = 1;

        if(debug) printf("IN: fft_sender:get_samples(%d), AFTER DISCARDING WAVE HEADER\n", N);
        // free(wav_header);
    }

    int i = 0, j;
    long long int n;
    int2 discard_sample;
    /* Read the first samplesToNextFFT # of samples to discard */
    for(j = 0; j < hdr->samplesToNextFFT; j++){
        if(sizeof(int2) != (n = read(fileno(stdin), &discard_sample, sizeof(int2)))){
           if(debug) printf("Discarding %lld bytes\n", n);
            err("Error discarding audio samples");
        }
    }

    char channel_sample;
    /* Read the next N samples, and separate left and right channels */
    for(j = 0; j < (N*2); j++){
        if(sizeof(char) == read(fileno(stdin), &channel_sample, sizeof(char))){
            // Only read in left channel data..
            // if((j & 1) != channel)
            if((j & 1) != 1)
                dbuffer[i++] = channel_sample;            
        } else {
            err("Error reading audio data");
        }
    }
}

/**
 * Channel: 1 = left, 0 = right
 */
int write_audio(int newsockfd, int N, int fftRate, double target, double weight, char agc_off, char channel)
{
    if(debug) printf("IN: fft_sender:write_audio(%d, %d)\n", newsockfd, N);

    double dbuffer[N];
    double out[N/2+1];

    if(debug) printf("IN: fft_sender:write_audio(): sending data\n");
    while(1){

        //if(debug) printf("IN: fft_sender:write_audio(): getting %d samples..\n", N);
        
        get_samples(N, dbuffer, fftRate, channel);

        if(debug)fprintf(stderr, "Sending header... ");
        int n = write(newsockfd, (char *) hdr, sizeof(fft_header));
        if(debug)fprintf(stderr, "Sent header, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");
        
        genfft(N, dbuffer, out, target, weight, agc_off);

        // int j;
        //if(debug) printf("Output buffer to send:\n");
        // for(j = 0; j < N; j++){
        //    if(debug) printf("out[%d] = %f\n", j, out[j]);
        // }
        
        // printf("Sending data.. out[255]: %f\n", out[255]);

        if(debug) printf("Sending fft data\n");
        // if(debug)fprintf(stderr, "Sending data... ");
        n = write(newsockfd, out, (N/2+1) * sizeof(double));
        if(debug) printf("Sent data, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");

    }

    free(hdr);
    close(newsockfd);
}

