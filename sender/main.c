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
    sprintf(syscall, "arecord -I -r44100 -D plughw:CARD=I82801AAICH  | sender_child %s", serverIP);
    system(syscall);
}

/*
 * Discard the header of the wave file, which contains unwanted meta-data. *  
 */
int discard_wav_header(float* buffer)
{   
    // WAV headers contained 46 byts of overhead:
    //  https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
    int n = fgets(buffer, 46, (int) stdin);
    return n;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
    {
     fprintf(stderr,"ERROR, no host provided\n");
     exit(1);
    }

    float temp[46];
    discard_wav_header(temp);

    start_audio(argv[1]);

}