/* fft_sender.c
 *
 * This file will contain functions that will package fft data to be sent 
 * through a socket.
 * 
 * Author: Ahmed Ismail
 */

#include <stdlib.h>
#include <sys/time.h>
#include "fft_sender.h"

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






