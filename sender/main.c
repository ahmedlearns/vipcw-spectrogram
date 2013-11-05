/*-------------------------------------------------------------------
 * main.c 
 * 
 * Author: Ahmed Ismail
 *-----------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

/* 
 * Need to make two binaries in the make file: sender & sender_child.
 *  sender will be the main entry point (this main file) and 
 *  sender_child will be the main in fft_sender.
*/

void start_audio(char* serverIP)
{   
    printf("IN: main:start_audio()\n");

    /* Execute arecord to start recording from the make with the separate-channels option */
    char syscall[512];
    // sprintf(syscall, "arecord -f S16_LE -r44100 -D plughw:CARD=Snowflake | tee sender.wav | ./sender_child %s", serverIP);
    sprintf(syscall, "arecord -f S16_LE -r44100 -D plughw:CARD=Snowflake | ./sender_child %s", serverIP);
    system(syscall);
}

/*
 * Discard the header of the wave file, which contains unwanted meta-data. *  
 */
int discard_wav_header()
{   
    // WAV headers contained 46 byts of overhead:
    //  https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
    float buffer[46];
    return fgets(buffer, 46, (int) stdin);
}

int main(int argc, char *argv[])
{
    printf("IN: main:main(%s)\n", argv[1]);

	if (argc < 2)
    {
        fprintf(stderr,"ERROR, no host provided\n");
        exit(1);
    }

    /* Don't need to discard header if we are manually separating the channels. */
    // if(!discard_wav_header())
    //     fprintf(stderr, "ERROR, discard wav header failed\n");

    start_audio(argv[1]);

}
