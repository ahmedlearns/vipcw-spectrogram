/**
 * monofft.h
 */

#include <fftw3.h>

typedef short signed int int2;

typedef struct wave {
    int chunkID;
    int chunkSize;
    int format;
    int subChunk1ID;
    int subChunk1size;
    // int audio_num_ch;
    short audioFormat;
    short numChannels;
    int sampleRate;
    int byteRate;
    short blockAlign;
    short bitsPerSample;
    // int bckAli_bitPs;
    int subChunk2ID;
    int subChunk2size;
    // int data[(lSize-40)/4];
} wave;

struct agc_params{
    int nc;
    double target;
    double weight;
};

void agc(struct agc_params, fftw_complex* out, double* output);
void genfft(int, double*, double*, double, double, char);
