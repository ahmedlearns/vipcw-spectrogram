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

void init_fft(int ptsPerFFT, struct timeval timestamp, int sampFreq)
{
	hdr = (struct fft_header*) malloc(sizeof(struct fft_header));
	hdr->ptsPerFFT = ptsPerFFT;
	hdr->timestamp = timestamp;
	hdr->sampFreq = sampFreq;

	// RETURN int ERROR CODES
}

// return buffer with fft data with header coming before it
struct fft_header * pack_fft(float * data){
	// Place header before the data;
	struct fft_header * header = (struct fft_header*) data) -1;
	return header = hdr;
}






