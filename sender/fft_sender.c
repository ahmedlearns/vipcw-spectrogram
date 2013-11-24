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

int debug = 0;

fft_header* hdr; 

void err(const char *msg)
{
    if(debug)fprintf(stderr, "%s\n", msg);
    exit(0);
}

int main(int argc, char *argv[])
{

    if(debug) printf("IN: fft_sender:main()\n");
    // int n = Write(get_samples(256), argv[1]);
    if(!write_audio(argv[1], 256));
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
    hdr->bytesToNextHeader = sizeof(float) * ptsPerFFT + sizeof(fft_header);
    hdr->samplesToNextFFT = samplesToNextFFT;
    hdr->ptsPerFFT = ptsPerFFT;
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
void get_samples(int N, double* dbuffer)
{

    if(debug) printf("IN: fft_sender:get_samples(%d)\n", N);
    
    static int init_samples = 0;
    if(!init_samples)
    {
        sleep(1);

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

        int bytesToNextHeader = N * sizeof(double) + sizeof(fft_header);  // total amount of space (header+data)
        // int sampleRate = wav_header->sampleRate;
        int sampleRate = 22050;
        int ptsPerFFT = N;         // number of points per FFT 
        int fftRate = 10;       // 10 FFT's per second.
        int samplesToNextFFT = (sampleRate / fftRate) - ptsPerFFT;   // Num samples to the start of the next FFT
        int endTrans = 5;

        init_fft(bytesToNextHeader, samplesToNextFFT, ptsPerFFT, sampleRate, fftRate, endTrans);
       if(debug) printf("\t Back in get_samples() from init_fft()\n");
        // sleep(1);
        init_samples = 1;

        // free(wav_header);
    }

    if(debug) printf("IN: fft_sender:get_samples(%d), AFTER DISCARDING WAVE HEADER\n", N);
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
            if((j & 1) != 1)
                dbuffer[i++] = channel_sample;            
        } else {
            err("Error reading audio data");
        }
    }
}

int write_audio(char* host, int N)
{
    if(debug) printf("IN: fft_sender:write_audio(%s, %d)\n", host, N);

    double dbuffer[N];
    double out[N];

    int sockfd, portno, n, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    // struct hostent *server;  
    socklen_t clilen;

    // if (argc < 2)
    // {
    //  if(debug)fprintf(stderr,"ERROR, no host provided\n");
    //  exit(1);
    // }
    portno = 51717;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error1("ERROR opening socket");
    //server = gethostbyname(argv[1]);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
               error1("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
        
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
            error1("ERROR on accept");

    /*bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        if(debug)fprintf(stderr, "Error on connect(): %s\n", strerror(errno));
        exit(-1);
    }*/

    if(debug) printf("IN: fft_sender:write_audio(): sending data\n");
    while(1){

        //if(debug) printf("IN: fft_sender:write_audio(): getting %d samples..\n", N);
        
        get_samples(N, dbuffer);

        if(debug)fprintf(stderr, "Sending header... ");
        n = write(newsockfd, (char *) hdr, sizeof(fft_header));
        if(debug)fprintf(stderr, "Sent header, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");
        
        genfft(N, dbuffer, out);

        // int j;
        //if(debug) printf("Output buffer to send:\n");
        // for(j = 0; j < N; j++){
        //    if(debug) printf("out[%d] = %f\n", j, out[j]);
        // }
        
        printf("Sending data.. out[255]: %f\n", out[255]);

        if(debug) printf("Sending fft data\n");
        // if(debug)fprintf(stderr, "Sending data... ");
        n = write(newsockfd, out, N * sizeof(double));
        if(debug) printf("Sent data, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");

    }

    free(hdr);
    close(newsockfd);
    close(sockfd);
}

