#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "fft_socket_header.h"
#include "linkedlist.h"

//#define AXIS_START 0
char buff[20];
double *buffer;
int samp_rate;
int CAMERA_WIDTH;
int port_num;
int sockfd;
int n;
int count;
int size;
int length;
int readCount = 0;

char *tempBuf;
int bufSize;
int getData();

struct sockaddr_in serv_addr;

struct fft_header header;
struct fft_header tempHeader;

//variables for multiclient server
int sockfd1, portno1, n1, newsockfd1;
struct sockaddr_in serv_addr1, cli_addr1;
    // struct hostent *server;  
socklen_t clilen1;

List * list;

void error1(const char *msg)
{
    fprintf(stderr, "%s: %s \n", msg, strerror(errno));
    exit(1);
}


int getData()
{
        int i;
        int constant;

        // get as many bytes in the socket to fill up the buffer
        n = recv(sockfd, tempBuf + readCount, length - readCount, MSG_DONTWAIT);        
        if(n>0)
                readCount += n;

        if(readCount == length)        //when get enough data
        {
                // check header constant 
                constant = ((int*)(tempBuf))[0];
                fprintf(stderr, "\nReading header... ");  
                printf("header.constSync is %X\n", constant);
                if(constant != 0xACFDFFBC)
                        error1("ERROR reading from socket, incorrect header placement\n");

                //put data into a buffer
               // for( i = 0 ; i < samp_rate; i++)
                 //       buffer[i] = ((double*)(tempBuf + sizeof(struct fft_header)))[i];
                //fprintf(stderr, "Reading data... ");

                //shift
                // call send to reciever here
                readCount = 0;
                if (list->head != NULL)
                	writeToClients();
        }
        return 1;
}

// writes the data in the buffer to all clients connected 
// to the server (2nd socket)
int writeToClients(){
	// head will never be NULL
	Node *ln = list->head;
	int x = ln->data;
	n = write(x, tempBuf, length);
	if (n < 0) 
             err("ERROR writing to socket");
    //written to first client
	
	while(ln->next != NULL){
		ln = ln->next;
		int x = ln->data;
		n = write(x, tempBuf, length);
		if (n < 0) 
             err("ERROR writing to socket");
	}
	
	 //newsocket for client, tempbuffer, length 
    //n = write(newsockfd, tempbuffer, length);
	//if (n < 0) 
      //       err("ERROR writing to socket");
	

}



int main(int argc, char *argv[])
{
        printf("main method running %d argc", argc);

        int i, j, k;
        count = 0;
        struct hostent *server;
        int sync = 0;

        
        //if (argc < 2)
        //{
             //fprintf(stderr,"ERROR, no host provided\n");
        //}
        /*Initialize socket*/
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (sockfd < 0)
                error1("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    port_num = 51717;
    serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(port_num);
            
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    {
                error1("Error on connect(), exitting");
            exit(-1);
    }
        
        //Synchronization
        fprintf(stderr, "Syncing... ");  
        
        n = read(sockfd, &header, sizeof(struct fft_header));
        if( n == 0)
        {
                printf("Sender has closed connection1\n");
                exit(0);
        } else if( n > 0)
        {
                printf("Read %d bytes from header, header.constSync is %X\n", n, header.constSync);        
        }
        do
        {
                for(i = 0; i < sizeof(struct fft_header)-7; i++)
                {
                        if(header.constSync != 0xACFDFFBC)
                        {
                                memcpy(&tempHeader, ((char*)(&header))+ 1, sizeof(struct fft_header) - 1 - i);
                                memcpy(&header, &tempHeader, sizeof(struct fft_header));
                        
                        }
                        else
                        {
                                sync = 1;
                                i++;
                                break;
                        }        
                        memset(&tempHeader, 0, sizeof(struct fft_header));
                        printf("%0x\n", header.constSync);                
                }
                if(i > 1)
                {
                        n = read(sockfd, &header + sizeof(struct fft_header) - 1- i,i - 1);
                        if( n == 0)
                        {
                                printf("Sender has closed connection1\n");
                                exit(0);
                        }
                }

                
        }while(!sync);

        if(header.constSync != 0xACFDFFBC)
                        error1("ERROR reading from socket, incorrect header placement\n");
        
        fprintf(stderr, "Synced... \n");  
        
        /*
        //First Header
            fprintf(stderr, "Reading header... ");  
            n = read(sockfd, &header, sizeof(struct fft_header));
            if (n < 0)
                       error1("ERROR reading from socket");
            else if (n > 0)
        {
                printf("header.constSync is %X\n", header.constSync);
                if(header.constSync != 0xACFDFFBC)
                        error1("ERROR reading from socket, incorrect header placement\n");
        }
        else if( n == 0)
        {
                printf("Sender has closed connection\n");
                exit(0);
        }
        */

        //Initializing structures
        samp_rate = header.ptsPerFFT;
        
        buffer = malloc(sizeof(double) * samp_rate);
        bufSize = sizeof(double)* samp_rate;
        length = (sizeof(struct fft_header) + sizeof(double)* samp_rate);
        size =  length* 2 + 1;
        tempBuf = malloc(length);

        //First Data
        fprintf(stderr, "Reading data... ");
            n = read(sockfd, (char *) buffer, header.ptsPerFFT * sizeof(double));
        if (n < 0)
                error1("ERROR reading from socket");
        else if( n == 0)
        {
                printf("Sender has closed connection\n");
                exit(0);
        }
        else 
        {
                printf("Read %d bytes data\n", n);
                printf("checkpoint 1\n");
        }
        
        /*End*/
        
        // socket for server 
        // socket setup
        
       

	printf("Setting up server\n");
	
    // if (argc < 2)
    // {
    //  if(debug)fprintf(stderr,"ERROR, no host provided\n");
    //  exit(1);
    // }
    portno1 = 51718;
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) 
       err("ERROR opening socket");
    //server = gethostbyname(argv[1]);
    bzero((char *) &serv_addr1, sizeof(serv_addr1));
    serv_addr1.sin_family = AF_INET;
    serv_addr1.sin_addr.s_addr = INADDR_ANY;
    serv_addr1.sin_port = htons(portno1);
    if (bind(sockfd1, (struct sockaddr *) &serv_addr1, sizeof(serv_addr1)) < 0)
               err("ERROR on binding");
        printf("Listening for connections..\n");
    listen(sockfd1,5);
    clilen1 = sizeof(cli_addr1);
    
    int flags = fcntl(sockfd1, F_GETFL, 0);//set the new socket to non blocking
	fcntl(sockfd1, F_SETFL, flags | O_NONBLOCK);
    
    list = emptylist();	// initialize the linked list that will contain the client types
    
    while(1){
    	//listen(sockfd1,5);
    	//printf("Listening....\n");

    	clilen1 = sizeof(cli_addr1);
    	newsockfd1 = accept(sockfd1, (struct sockaddr *) &cli_addr1, &clilen1); 
    	printf(" file descriptor value %d \n",newsockfd1);

    	if (newsockfd1 < 0)
            printf("No connection to client\n");   
        if(newsockfd1 != EAGAIN && newsockfd1 !=EWOULDBLOCK && newsockfd1 >= 0){
        	printf("Accepted a connection..\n");
    		printf(" file descriptor value %d \n",newsockfd1);
        	add(newsockfd1,list);
        	printf("Added a client file descriptor value %d \n",newsockfd1);

        }
        getData();          
    }    
        
        
        //newsocket for client, tempbuffer, length 
        //n = write(newsockfd, out, (N/2+1) * sizeof(double));
        
        //loadeImage function

        //call shift every 50msec
      
   
    return 0;

}
