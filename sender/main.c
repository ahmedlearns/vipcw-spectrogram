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
    /* Execute arecord to start recording from the make with the separate-channels option */
    char syscall[512];
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
	if (argc < 2)
    {
        fprintf(stderr,"ERROR, no host provided\n");
        exit(1);
    }

    if(!discard_wav_header())
        fprintf(stderr, "ERROR, discard wav header failed\n");

    start_audio(argv[1]);

}
