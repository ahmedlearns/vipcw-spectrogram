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
void start_audio(int newsockfd)
{   
    printf("IN: main:start_audio()\n");

    /* Execute arecord to start recording from the make with the separate-channels option */
    char syscall[512];
    // sprintf(syscall, "arecord -f S16_LE -r44100 -D plughw:CARD=Snowflake | tee sender.wav | ./sender_child %s", serverIP);
    // sprintf(syscall, "arecord -f S16_LE -r22050 -D hw:CARD=AudioPCI | ./sender_child %s", serverIP);
    sprintf(syscall, "arecord -f S16_LE -r22050 -D front:CARD=Snowflake | ./sender_child %d", newsockfd);
    system(syscall);
}


int main(int argc, char *argv[])
{
    printf("IN: main:main()\n");

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
        start_audio(newsockfd);
        
    // if (argc < 2)
 //    {
 //        fprintf(stderr,"ERROR, no host provided\n");
 //        exit(1);
 //    }


}
