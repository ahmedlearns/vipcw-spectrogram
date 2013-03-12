/*
    stereo2mono.c

    Convert a Stereo file into a Mono wave file by concatenating the left channel data together.
    Write the sound data to a text file to understand what's going on.
    Write the sound data to an excel file that will be used with matLab to compare.
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{   int i=0,j;
   
    int lSize;
    FILE * f = fopen("wavs/test.wav", "r"); //opening the 2 channels wave file
    FILE * f1 = fopen ("testMono.wav", "wb"); // this is  the file where the one channel sound file will be saved
    FILE * f2 = fopen ("testSample.txt", "w"); // a text file that will have the left channel sound data,
                                                 //just to see what we're dealing with
    FILE * f3 = fopen ("testSample.csv", "w");  // an excel file that have the data to use in matLab
    
    /* Find size of wav file */
    fseek (f , 0 , SEEK_END);   // set current position of the stream to the end of the file
    lSize = ftell (f);      // number of bytes from the beginning of the file to the end
    rewind (f);             // reset the current position of the file stream
    
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
                int  data[(lSize-40)/4];    // DOES THIS NEED TO BE lSize-44? The "/4" is because it's 1 word (32 bits) at a time
                } wave;
    wave * mywave;
    mywave=(wave *) malloc(lSize);

    fread(mywave,1,lSize,f);            // load the whole file into the mywave buffer


    i=0,j=0;
    int mask = 65535;   // 1111111111111111 -> 16 binary 1's
    int leftVa;         // left channel parameter
    while (i<((lSize-40)/4)){   // loop through each segment of data
        leftVa = (mywave->data[i+1] << 16) | (mywave->data[i] & mask);  // mask out left channel value
        mywave->data[j]=leftVa;     // store left channel data in mywave
        fprintf(f2,"%x\n",  leftVa);    // Keep track of the sound data in hex, for viewing/debugging
        fprintf(f3,"%d\n",  leftVa);    // Log decimal values for use in matlab
        j++;
        i=i+2;
    }

    mywave->audio_num_ch=mywave->audio_num_ch-65536;
    mywave->chunkSize=(mywave->chunkSize/2)+18;         // Not sure about the '+18'
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
