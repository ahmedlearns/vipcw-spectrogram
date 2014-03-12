/*-------------------------------------------------------------------
 * main.c 
 * 
 * Author: Ahmed Ismail
 *-----------------------------------------------------------------*/

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

int debug = 1;

char* DEFAULT_D = "default";

/* 
 * Need to make two binaries in the make file: sender & sender_child.
 *  sender will be the main entry point (this main file) and 
 *  sender_child will be the main in fft_sender.
*/

// void start_audio(char* serverIP)

/**
 *  Initiate audio recording and pass needed socket information.
 *
 *
 */
void start_audio(int newsockfd, char* d, int r, int s, int f, char a, double t, double w)
{   
    printf("IN: main:start_audio()\n");

    printf("n: %d\n", newsockfd);

    /* Execute arecord to start recording from the make with the separate-channels option */
    char syscall[512];
    // sprintf(syscall, "arecord -f S16_LE -r44100 -D plughw:CARD=Snowflake | tee sender.wav | ./sender_child %s", serverIP);
    // sprintf(syscall, "arecord -f S16_LE -r22050 -D hw:CARD=AudioPCI | ./sender_child %s", serverIP);
    sprintf(syscall, "arecord -f S16_LE -r%d -D %s | ./sender_child -s %d -f %d -n %d -a %d -t %f -w %f", r, d, s, f, newsockfd, a, t, w);
    system(syscall);
}


void print_help_and_exit(void) {
    printf("sender [OPTIONS]\n");
    printf("  -d [device]\t\tInput Device ('arecord -L')\tDefault: default\n");
    printf("  -p [portno]\t\tDestination Port Number\t\tDefault: 51717\n");
    printf("  -r [sampFreq]\t\tRecording Sampling Rate\t\tDefault: 22050 Hz\n");
    printf("  -s [ptsPerFFT]\tNo. of points per FFT\t\tDefault: 256\n");
    printf("  -f [fftFreq]\t\tNo. of FFT's per second\t\tDefault: 10\n");
    printf("  -a \t\t\tTurn Off AGC\t\tDefault: On\n");
    printf("  -t [agc_target]\tTarget FFT Amplitude [0-1]\tDefault: 0.75\n");    
    printf("  -w [agc_weight]\tWeight of prev. AGC input [0-1]\tDefault: 0.99\n");
    printf("  -h\t\t\tThis helpful output\n");
    exit(0);
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
    char a = DEFAULT_A;

    /* Read arguments */ 
    while(-1 != (opt = getopt(argc, argv, "d:p:r:s:f:at:w:h"))) {
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
        case 'a':
            a = 1;
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
    printf("s: %d\n", s);
    printf("f: %d\n", f);
    printf("a: %d\n", a);
    printf("t: %.3f\n", t);
    printf("w: %.3f\n", w);
    printf("\n");

    int sockfd, portno, n, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    // portno = 51717;
    portno = p;
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
        start_audio(newsockfd, d, r, s, f, a, t, w);


}
