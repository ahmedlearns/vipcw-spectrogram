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
float* fbuffer;

//void init_fft(int bytesToNextHeader, int samplesToNextFFT, int ptsPerFFT, 
//		struct timeval timestamp, int sampFreq)
void init_fft(int bytesToNextHeader, int samplesToNextFFT, int ptsPerFFT, 
			 int sampFreq, int endTrans)
{
    printf("IN: fftsender:init_fft()\n");
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
    printf("IN: fftsender:main()\n");
    // int n = Write(get_samples(256), argv[1]);
    if(!Write(argv[1]), 256);
        fprintf(stderr, "ERROR, write failed\n");
    return 0;
}



// float* get_samples(int N)
void get_samples(int N)
{

    printf("IN: fft_sender:get_samples(%d)\n", N);

    int flags = fcntl(stdin, F_GETFL);  /* get current file status flags */
    flags |= O_NONBLOCK;                /* turn off blocking flag */
    fcntl(stdin, F_SETFL, flags);       /* set up non-blocking read */

    static int init_samples = 0;
    if(!init_samples)
    {
        int bytesToNextHeader = 5;  // total amount of space (header+data)
        int samplesToNextFFT = 3;   // Num samples to the start of the next FFT
        int ptsPerFFT = N;         // number of points per FFT 
        // srand(time(NULL)); int ptsPerFFT = (int) rand() % 20;         // number of points per FFT 
        int sampFreq = 4;
        int endTrans = -1;
        init_fft(bytesToNextHeader, samplesToNextFFT, ptsPerFFT, sampFreq, endTrans);
        init_samples = 1;
    }

    float a[N], b[N];
    int na, nb, n = 0;
    // float *ptr = a;
    fbuffer = a;

    // While N samples have been read:
    while(n == N){
        n =  fgets(fbuffer, N*sizeof(float), stdin);
        if(n == N){
            fbuffer = (fbuffer == a) ? b : a;
            // if(fbuffer == a) fbuffer = b;
            // else fbuffer = a;
        } else {
            return (fbuffer == a) ? b : a;
        }
    }

    // int rcvd_a, rcvd_b;
    // rcvd_a = fgets(buff_a, ptsPerFFT*sizeof(float), (int) stdin);
    // rcvd_b = fgets(buff_b, ptsPerFFT*sizeof(float), (int) stdin);

    // if(rcvd_a < ptsPerFFT*sizeof(float))
    //     return 'a';
    // if(rcvd_b < ptsPerFFT*sizeof(float))
    //     return 'b';

    // return 'x';

}

int Write(char* host, int N)
{
    printf("IN: fftsender:Write(%s)\n", host);

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

    printf("IN: fftsender:Write(): sending data\n");
    while(1){
        
        // fbuffer = get_samples(int ptsPerFFT);

        // char choose_buffer = get_samples(buff_a, buff_b);

        // if(choose_buffer == 'a')
        //     fbuffer = buff_a;
        // else if(choose_buffer == 'b')
        //     fbuffer = buff_b;
        // else 
        // {
        //     printf("Both buffers are not full.\n");
        //     fbuffer = buff_a;
        // }

        fprintf(stderr, "Sending header... ");
        n = write(sockfd, (char *) hdr, sizeof(struct fft_header));
        fprintf(stderr, "Sent header, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");
        
        get_samples(N);
        genfft(fbuffer, hdr->ptsPerFFT);
        
        printf("Sending fbuffer\n");
        fprintf(stderr, "Sending data... ");
        n = write(sockfd, fbuffer, hdr->ptsPerFFT * sizeof(float));
        fprintf(stderr, "Sent data, n = %d\n", n);
        if (n < 0) 
             err("ERROR writing to socket");

        return n;
    }
    close(sockfd);
}

