#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(){
	int lSize;
	typedef short unsigned int int2;

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
	
	mkfifo("wavePipe", S_IRWXO);
    system("arecord -f S16_LE -r22050 -D hw:CARD=AudioPCI wavePipe");
    // system("arecord -d1 -r4000 -D sysdefault:CARD=U0x46d0x80a wavePipe");
    FILE * f = fopen("wavePipe", "r"); //opening the 2 channels wave file
        //usleep(10000);
    //FILE * f = fopen("chirp10-500Hz.wav", "r"); //opening the 2 channels wave file
    if(!f) printf("Error reading from wavePipe");

    // FILE * f2 = fopen ("chirpFFT.txt", "w"); // a text file that will have the left channel sound data,
    //just to see what we're dealing with
    fseek (f , 0 , SEEK_END);
    lSize = ftell (f);
    rewind (f);

    wave * mywave;
    mywave=(wave *) malloc(lSize);

    fread(mywave,1,48,f);

    printf("wavePipe is %d kB\n", lSize / 1024);
    printf("sampleRate is %d Hz\n", mywave->sampleRate);
    printf("bits per sample is %d\n", mywave->bitsPerSample);
	// printf("sizeof(int2) is %d\n", sizeof(int2));

}