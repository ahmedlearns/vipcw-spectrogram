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
    sprintf(syscall, "arecord -f S16_LE -r22050 -D hw:CARD=AudioPCI | ./sender_child %s", serverIP);
    system(syscall);
}


int main(int argc, char *argv[])
{
    printf("IN: main:main(%s)\n", argv[1]);

	if (argc < 2)
    {
        fprintf(stderr,"ERROR, no host provided\n");
        exit(1);
    }

    start_audio(argv[1]);

}
