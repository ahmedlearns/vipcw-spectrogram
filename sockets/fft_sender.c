/* fft_sender.c
 *
 * This file will contain functions that will package fft data to be sent 
 * through a socket.
 * 
 * Author: Ahmed Ismail
 * Version: 1.0 2013-3-20
 */

#include <stdlib.h>
#include <sys/time.h>

#include "fft_socket_header.h"

struct fft_header * hdr;

void init_fft(int bytesToNextHeader, int ptsPerFFT, 
		struct timeval timestamp, int sampFreq)
{
	hdr = (struct fft_header*) malloc(sizeof(struct fft_header));
	hdr->bytesToNextHeader = sizeof(float) * ptsPerFFT + sizeof(struct fft_header);
	hdr->ptsPerFFT = ptsPerFFT;
	updateTime(timestamp);
	hdr->sampFreq = sampFreq;

	// RETURN int ERROR CODES
}

void updateTime(struct timeval timestamp){
	gettimeofday(hdr->timestamp, NULL);
}






