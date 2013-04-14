/* fft_receiver.c
 *
 * This file will contain functions that will parse incoming fft data from
 * a socket.
 * 
 * Author: Ahmed Ismail
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "fft_socket_header.h"

struct fft_header header;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    // char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    //~ if (argc < 2)
    //~ {
     //~ fprintf(stderr,"ERROR, no port provided\n");
     //~ exit(1);
    //~ }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
     error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    //~ portno = atoi(argv[1]);
    portno = 51717;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,              sizeof(serv_addr)) < 0)
           error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    // Try to write server that accepts multiple connections
    /*
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
       error("ERROR on accept"); 
    */
   
    // bzero(buffer,256); 
    //   n = read(newsockfd,buffer,255); 
    //   if (n < 0)
    // error("ERROR reading from socket");
    //printf("Here is the message: %s\n",buffer);
//////////////////////////////////////////////////////////////////  
    float fbuffer[256];
    while( newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)){
        printf("Connection accepted\n");

        n = read(newsockfd, &header, sizeof(struct fft_header));
        if (n < 0)
            error("ERROR reading from socket");
        else if (n > 0){
            printf("header.constSync is %X\n", header.constSync);
            // printf("header_len is %d\n", sizeof(struct fft_header));

            if(header.constSync != 0xACFDFFBC)
                error("ERROR reading from socket, incorrect header placement\n");
        }

        n = read(newsockfd, (char *) fbuffer, header.ptsPerFFT * sizeof(float));
        if (n < 0)
            error("ERROR reading from socket");
        else if(n > 0){
            int i;
            printf("ptsPerFFT: %d\n", header.ptsPerFFT);
            for(i = 0; i < header.ptsPerFFT; i++)
                printf("%d: %f\n", i+1, fbuffer[i]);
        }
    }
    if (newsockfd < 0)
       error("ERROR on accept"); 
    /*
    while(1){
        n = read(newsockfd, &header, sizeof(struct fft_header));
        if (n < 0)
            error("ERROR reading from socket");
        else if (n > 0){
			printf("header.constSync is %X\n", header.constSync);
            // printf("header_len is %d\n", sizeof(struct fft_header));

			if(header.constSync != 0xACFDFFBC)
				error("ERROR reading from socket, incorrect header placement\n");
		}

        n = read(newsockfd, (char *) fbuffer, header.ptsPerFFT * sizeof(float));
        if (n < 0)
            error("ERROR reading from socket");
        else if(n > 0){
			int i;
			printf("ptsPerFFT: %d\n", header.ptsPerFFT);
			for(i = 0; i < header.ptsPerFFT; i++)
				printf("%d: %f\n", i+1, fbuffer[i]);
		}
        
        //~ else if(n == 0)
			//~ error("Socket has closed");

        //~ printf("header.endTrans is %d\n", header.endTrans);

    } 
    */    
   // } while(header.endTrans < 0);

	/*
    n = read(newsockfd, &header, sizeof(struct fft_header));
	if (n < 0)
		error("ERROR reading from socket");

    if(header.constSync != 0xACFDFFBC)
        error("ERROR reading from socket: Incorrect header placement\n");
	
	
	n = read(newsockfd, (char *) fbuffer, header.ptsPerFFT * sizeof(float));
	if (n < 0)
		error("ERROR reading from socket");
	*/
	

	// FILE * f =fopen("data.txt","w");
	// fwrite(buffer,1,255,f);
	// fclose(f);
/////////////////////////////////////////////////////////////////

     // n = write(newsockfd,"I got your message",18);
     // if (n < 0) error("ERROR writing to socket");
     
     close(newsockfd);
     close(sockfd);
     return 0;
 }

