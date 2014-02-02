/*-------------------------------------------------------------------
 * main.c 
 * 
 * Author: Ahmed Ismail
 *
 * Need to make two binaries in the make file: sender & sender_child.
 *  sender will be the main entry point (this main file) and 
 *  sender_child will be the main in fft_sender.
 *
 ------------------------------------------------------------------*/

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

 int debug = 0;

void print_help_and_exit(void) {
    printf("sender [OPTIONS]\n");
    printf("  -d device\t\tInput Device ('arecord -L')\tDefault: default\n");
    printf("  -p portno\t\tDestination Port Number\t\tDefault: 51717\n");
    printf("  -r sampFreq\t\tRecording Sampling Rate\t\tDefault: 22050 Hz\n");
    printf("  -s ptsPerFFT\t\tNo. of points per FFT\t\tDefault: 256\n");
    printf("  -f fftFreq\t\tNo. of FFT's per second\t\tDefault: 10\n");
    printf("  -t agc_target\t\tTarget FFT Amplitude [0-1]\tDefault: 0.75\n");    
    printf("  -w agc_weight\t\tWeight of prev. AGC input [0-1]\tDefault: 0.99\n");
    printf("  -h\t\t\tThis helpful output\n");
    exit(0);
}

/* Initiate audio recording and pass needed socket information. */
void start_audio(int newsockfd, char* d, int p, int r, int s, int f, double t, double w)
{   
    printf("IN: main:start_audio()\n");

    /* Execute arecord to start recording from the make with the separate-channels option */
    char syscall[512];
    sprintf(syscall, "arecord -f S16_LE -r%d -D %s | ./sender_child -r %d -s %d -f %d -t %f -w %f -n %d", r, d, r, s, f, t, w, newsockfd);
    system(syscall);
}


int main(int argc, char *argv[])
{
    if(debug)
        printf("IN: main:main()\n");

    int opt;
    char* d = DEFAULT_D;
    int p = DEFAULT_P;
    int r = DEFAULT_R;
    int s = DEFAULT_S;
    int f = DEFAULT_F;
    double t = DEFAULT_T;
    double w = DEFAULT_W;

    /* Read arguments */ 
    while(-1 != (opt = getopt(argc, argv, "d:p:r:s:f:t:w:h"))) {
        switch(opt) {
        case 'd':
            d = optarg;
            break;
        case 'p':
            p = atoi(optarg);
            break;
        case 'r':
            r = atoi(optarg);
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'f':
            f = atoi(optarg);
            break;
        case 't':
            t = atof(optarg);
            break;
        case 'w':
            w = atof(optarg);
            break;
        case 'h':
            /* Fall through */
        default:
            print_help_and_exit();
            break;
        }
    }

    printf("Source Settings\n");
    printf("d: %s\n", d);
    printf("p: %d\n", p);
    printf("r: %d\n", r);
    printf("f: %d\n", f);
    printf("t: %.3f\n", t);
    printf("w: %.3f\n", w);
    printf("\n");

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
       err("ERROR opening socket");
    //server = gethostbyname(argv[1]);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
               err("ERROR on binding");
	printf("Listening for connections..\n");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
        
	printf("Accepting connection..\n");
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	printf("Accepted a connection..\n");
    if (newsockfd < 0)
            err("ERROR on accept");
    else 
        start_audio(newsockfd, d, p, r, s, f, t, w);
        
    // if (argc < 2)
 //    {
 //        fprintf(stderr,"ERROR, no host provided\n");
 //        exit(1);
 //    }


}
