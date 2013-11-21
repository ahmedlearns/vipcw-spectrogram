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

struct fft_header * hdr;

//void init_fft(int bytesToNextHeader, int samplesToNextFFT, int ptsPerFFT, 
//		struct timeval timestamp, int sampFreq)
void init_fft(int bytesToNextHeader, int samplesToNextFFT, int ptsPerFFT, 
			 int sampFreq, int fftRate, int endTrans)
{
    printf("IN: fft_sender:init_fft()\n");
	hdr = (struct fft_header*) malloc(sizeof(struct fft_header));
	hdr->constSync = 0xACFDFFBC;
	hdr->bytesToNextHeader = sizeof(float) * ptsPerFFT + sizeof(struct fft_header);
	hdr->ptsPerFFT = ptsPerFFT;
	hdr->samplesToNextFFT = samplesToNextFFT;
	//updateTime(timestamp);
	hdr->sampleRate = sampFreq;
	// -1 when ongoing, 1 to signal end of transmission
    hdr->fftRate = fftRate;
	hdr->endTrans = endTrans;

	// RETURN int ERROR CODES
}

// void updateTime(struct timeval timestamp){
	// gettimeofday(hdr->timestamp, NULL);
// }

void err(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    printf("IN: fft_sender:main()\n");
    // int n = Write(get_samples(256), argv[1]);
    if(!write_audio(argv[1], 256));
        fprintf(stderr, "ERROR, write failed\n");
    return 0;
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
void get_samples(int N, double* dbuffer)
{

    printf("IN: fft_sender:get_samples(%d)\n", N);
    static int init_samples = 0;
    if(!init_samples)
    {
        wave* wav_header = (wave*) malloc(sizeof(wave));
        /* Read in Wave header to set fft_header and then discard*/
        if(WAV_HEADER_SIZE != fgets(wav_header, WAV_HEADER_SIZE, stdin))
            err("Error discarding Wave header");

        int bytesToNextHeader = N * sizeof(double) + sizeof(struct fft_header);  // total amount of space (header+data)
        int sampleRate = wav_header->sampleRate;
        int ptsPerFFT = N;         // number of points per FFT 
        int fftRate = 10;       // 10 FFT's per second.
        int samplesToNextFFT = (sampleRate / fftRate) - ptsPerFFT;   // Num samples to the start of the next FFT
        int endTrans = -1;

        init_fft(bytesToNextHeader, samplesToNextFFT, ptsPerFFT, sampleRate, fftRate, endTrans);
        
        free(wav_header);
        init_samples = 1;
    }

    int n, i = 0, j;
    int2 discard_sample;
    /* Read the first samplesToNextFFT # of samples to discard */
    for(j = 0; j < hdr->samplesToNextFFT; j++){
        if(sizeof(int2) != fgets(&discard_sample, sizeof(int2), stdin))
            err("Error discarding audio samples");
    }

    char channel_sample;
    /* Read the next N samples, and separate left and right channels */
    for(j = 0; j < (N*2); j++){
        if(sizeof(char) == fgets(&channel_sample, sizeof(char), stdin)){
            // Only read in left channel data..
            if((j & 1) != 1)
                dbuffer[i++] = channel_sample;            
        } else {
            err("Error reading audio data");
        }
    }
}

int write_audio(char* host, int N)
{
    printf("IN: fft_sender:write_audio(%s, %d)\n", host, N);

    double dbuffer[N];
    double out[N];

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;  

    // if (argc < 2)
    // {
    //  fprintf(stderr,"ERROR, no host provided\n");
    //  exit(1);
    // }
    portno = 51717;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        err("ERROR opening socket");
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(-1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Error on connect(): %s\n", strerror(errno));
        exit(-1);
    }

    printf("IN: fft_sender:Write(): sending data\n");
    while(1){

        printf("IN: fft_sender:Write(): getting %d samples..\n", N);
        
        get_samples(N, dbuffer);

        fprintf(stderr, "Sending header... ");
        n = write(sockfd, (char *) hdr, sizeof(struct fft_header));
        fprintf(stderr, "Sent header, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");
        
        genfft(N, dbuffer, &out);
        
        printf("Sending fbuffer\n");
        fprintf(stderr, "Sending data... ");
        n = write(sockfd, out, hdr->ptsPerFFT * sizeof(double));
        fprintf(stderr, "Sent data, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");

        return n;
    }

    free(hdr);
    close(sockfd);
}

