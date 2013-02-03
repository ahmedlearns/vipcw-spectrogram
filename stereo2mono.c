/*
    stereo2mono.c

    Convert a Stereo file into a Mono wave file by concatenating the left channel data together.
    Write the sound data to a text file to understand what's going on.
    Write the sound data to an excel file that will be used with matLab to compare.
*/

#include <stdlib.h>

int main(int argc, char *argv[])
{   int i=0,j;
   
    int lSize;
    FILE * f = fopen("pianoStereo.wav", "r"); //opening the 2 channels wave file
    FILE * f1 = fopen ("pianoMono.wav", "wb"); // this is  the file where the one channel sound file will be saved
    aFILE * f2 = fopen ("pianoSample.txt", "w"); // a text file that will have the left channel sound data,
                                                 //just to see what we're dealing with
    FILE * f3 = fopen ("pianoSample.csv", "w");  // an excel file that have the data to use in matLab
    fseek (f , 0 , SEEK_END);
    lSize = ftell (f);
    rewind (f);
    
    typedef struct wave {
                int chunkID;
                int chunkSize;
                int format;
                int subChunk1ID;
                int subChunk1size;
                int audio_num_ch;
                int sampleRate;
                int byteRate;
                int bckAli_bitPs;
                int subChunk2ID;
                int subChunk2size;
                int  data[(lSize-40)/4];
                } wave;
    wave * mywave;
    mywave=(wave *) malloc( lSize);

    fread(mywave,1,lSize,f);


    i=0,j=0;
    int mask=65535;
    int  leftVa;
    while (i<((lSize-40)/4)){

    leftVa=(mywave->data[i+1]<<16) | (mywave->data[i] & mask);
    mywave->data[j]=leftVa;
    fprintf(f2,"%x\n",  leftVa);
    fprintf(f3,"%d\n",  leftVa);
    j++;
    i=i+2;
                            }

    mywave->audio_num_ch=mywave->audio_num_ch-65536;
    mywave->chunkSize=(mywave->chunkSize/2)+18;
    mywave->byteRate=mywave->byteRate/2;
    mywave->bckAli_bitPs=mywave->bckAli_bitPs-2;
    mywave->subChunk2size=mywave->chunkSize-36;


    fwrite(mywave,1,(lSize/2)+22,f1);
    fclose (f);
    fclose (f1);
    fclose (f2);
    fclose (f3);
    return 0;
 }