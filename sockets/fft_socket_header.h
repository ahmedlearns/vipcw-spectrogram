/* A file to structurally send fft data through a socket 
 *
 * To read in an FFT, you will first need to read in the header portion (the struct below)
 * The second read will be the fft_data and the size of the read will be dependent on the 
 * 	ptsPerFFT parameter of the previous read.
 */

#include <stdlib.h>
#include <sys/time.h>

struct fft_header{
	int ptsPerFFT;			// number of points per FFT
	struct timeval timestamp;	// see getimeofday -> "man gettimeofday"
	int sampFreq;			// sampling frequency
}
