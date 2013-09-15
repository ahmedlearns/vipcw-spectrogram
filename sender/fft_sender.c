/* fft_sender.c
 *
 * This file will contain functions that will package fft data to be sent 
 * through a socket.
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

struct fft_header * hdr;

//void init_fft(int bytesToNextHeader, int samplesToNextFFT, int ptsPerFFT, 
//		struct timeval timestamp, int sampFreq)
void init_fft(int bytesToNextHeader, int samplesToNextFFT, int ptsPerFFT, 
			 int sampFreq, int endTrans)
{
	hdr = (struct fft_header*) malloc(sizeof(struct fft_header));
	hdr->constSync = 0xACFDFFBC;
	hdr->bytesToNextHeader = sizeof(float) * ptsPerFFT + sizeof(struct fft_header);
	hdr->ptsPerFFT = ptsPerFFT;
	hdr->samplesToNextFFT = samplesToNextFFT;
	//updateTime(timestamp);
	hdr->sampFreq = sampFreq;
	// -1 when ongoing, 1 to signal end of transmission
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
    int bytesToNextHeader = 5;  // total amount of space (header+data)
    int samplesToNextFFT = 3;   // Num samples to the start of the next FFT
    int ptsPerFFT = 256;         // number of points per FFT 
    // srand(time(NULL)); int ptsPerFFT = (int) rand() % 20;         // number of points per FFT 
    int sampFreq = 4;
    int endTrans = -1;

    init_fft(bytesToNextHeader, samplesToNextFFT, ptsPerFFT, sampFreq, endTrans);
    int header_len = sizeof(struct fft_header);
      
    float fbuffer[ptsPerFFT];
    int i, j;
    int k = 0;
    
    int flags = fcntl((int) stdin, F_GETFL); /* get current file status flags */
    flags |= O_NONBLOCK;        /* turn off blocking flag */
    fcntl((int) stdin, F_SETFL, flags);       /* set up non-blocking read */

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
    server = gethostbyname(argv[1]);
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
     
    n = Write(sockfd, fbuffer, ptsPerFFT * sizeof(float), hdr, header_len);
    // while(1){
    //     fprintf(stderr, "Sending header... ");

    //     n = write(sockfd, (char *) hdr, header_len);
    //     fprintf(stderr, "Sent header, n = %d\n", n);
    //     if (n < 0) 
    //          err("ERROR writing to socket");
        
    //     // Generate random numbers to be sent each time.
    //     // srand(time(NULL));
    //     // for(i = 0; i < 256; i++){
    //     //     fbuffer[i] = (float) rand() / (float) RAND_MAX;
    //     // }

    //     genfft(fbuffer, ptsPerFFT, argv[1]);
        
    //     printf("Sending fbuffer\n");
    //     fprintf(stderr, "Sending data... ");
    //     n = write(sockfd, fbuffer, ptsPerFFT * sizeof(float));
    //     fprintf(stderr, "Sent data, n = %d\n", n);
    //     if (n < 0) 
    //          err("ERROR writing to socket");

    //     //usleep(500 * 1000);
    //     k++;
    // }
    close(sockfd);
    return 0;
}

/*
 * Discard the header of the wave file, which contains
 *  unwanted meta-data.
 */
int discard_wav_header(float* buffer)
{   
    // WAV headers contained 46 byts of overhead:
    //  https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
    int n = fgets(buffer, 46, (int) stdin);
    return n;
}

int Write(int sockfd, float* fbuffer, int numElements,
        struct fft_header *hdr, int header_len)
{
    discard_wav_header(fbuffer);

    int n;
    while(1){
        
        n = fgets(fbuffer, hdr->ptsPerFFT*sizeof(float), (int) stdin);

        fprintf(stderr, "Sending header... ");

        n = write(sockfd, (char *) hdr, header_len);
        fprintf(stderr, "Sent header, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");
        
        genfft(fbuffer, hdr->ptsPerFFT);
        
        printf("Sending fbuffer\n");
        fprintf(stderr, "Sending data... ");
        n = write(sockfd, fbuffer, hdr->ptsPerFFT * sizeof(float));
        fprintf(stderr, "Sent data, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");

        return n;
    }
}

