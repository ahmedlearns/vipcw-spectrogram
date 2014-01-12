typedef struct fft_header {
        int constSync;        // sync to next header
        int bytesToNextHeader;        // total amount of space (header+data)
        int samplesToNextFFT;        // Num samples to the start of the next FFT
        int ptsPerFFT;                        // number of points per FFT 
        //struct timeval timestamp;        // see getimeofday -> "man gettimeofday"
        int sampleRate;                        // sampling frequency
        int        fftRate;                // Number of FFT's per second
        int endTrans;        // signal end of transmission (all data is sent)
} fft_header;