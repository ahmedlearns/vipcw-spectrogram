/* fft_sender.c
 *
 * This file will contain functions that will package fft data to be sent 
 * through a socket.
 * 
 * Author: Ahmed Ismail
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// #include "fft_sender.h"
#include "fft_socket_header.h"

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
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // char buffer[256];
    //~ if (argc < 3) {
       //~ fprintf(stderr,"usage %s hostname port\n", argv[0]);
       //~ exit(0);
    //~ }
    //~ portno = atoi(argv[2]);
    if (argc < 2)
    {
     fprintf(stderr,"ERROR, no host provided\n");
     exit(1);
    }
    portno = 51717;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    

    ///////////////////////////////////////////////////////////////////////////////////////////
    // char a[3] = {'a','a','a'};
    // n = write(sockfd, a, 3);

    int bytesToNextHeader = 5;  // total amount of space (header+data)
    int samplesToNextFFT = 3;   // Num samples to the start of the next FFT
    //int ptsPerFFT = 20;         // number of points per FFT 
    srand(time(NULL)); int ptsPerFFT = (int) rand() % 20;         // number of points per FFT 
    int sampFreq = 4;
    int endTrans = -1;
    
    char fft_data[ptsPerFFT * sizeof(float)];

    init_fft(bytesToNextHeader, samplesToNextFFT, ptsPerFFT, sampFreq, endTrans);
    int header_len = sizeof(struct fft_header);
    
    /*
    n = write(sockfd, (char *) hdr, header_len);
    if (n < 0) 
         error("ERROR writing to socket");
    */

	// FILE * f =fopen("data.txt","w");
	// fwrite(fft_data, 1, 255, f);
	// fclose(f);
	
	float fbuffer[256];
	int i, j;
	//~ for(i = 0; i < 256; i++){
		//~ fbuffer[i] = 0.3*i;
	//~ }
	/*
	n = write(sockfd, fbuffer, 256 * sizeof(float));
    if (n < 0) 
         error("ERROR writing to socket");
    */

    // printf("header_len is %d\n", header_len);

    // DO WE WANT TO SEND THE DATA IN MANY SEPERATE SOCKETS OR IN ONE BIG SOCKET?


    n = write(sockfd, (char *) hdr, header_len);
    if (n < 0) 
         error("ERROR writing to socket");
    
    // Generate random numbers to be sent each time.
    srand(time(NULL));
    for(i = 0; i < 256; i++){
        fbuffer[i] = (float) rand() / (float) RAND_MAX;
    }
    n = write(sockfd, fbuffer, ptsPerFFT * sizeof(float));
        if (n < 0) 
             error("ERROR writing to socket");
    /*
    for(i = 0; i < 3; i++){
        
        //~ init_fft(bytesToNextHeader++, samplesToNextFFT+=2, ptsPerFFT, sampFreq, 
                    //~ endTrans);
        n = write(sockfd, (char *) hdr, header_len);
        if (n < 0) 
             error("ERROR writing to socket");
        
        for(j = 0; j < 256; j++){
           fbuffer[j] = 0.25*i;
        }
        n = write(sockfd, fbuffer, ptsPerFFT * sizeof(float));
        if (n < 0) 
             error("ERROR writing to socket");

		printf("This is iteration %d\n", i+1);

        //~ if(i == 2) endTrans = 1;
        //~ printf("endTrans is %d\n", endTrans);
    }
    */
	///////////////////////////////////////////////////////////////////////////////////////////
    

    close(sockfd);
    return 0;
}








