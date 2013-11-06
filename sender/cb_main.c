/*

	cb_main.c

*/

#include <stdlib.h>	

int main(){
	char syscall[512];
    // sprintf(syscall, "arecord -f S16_LE -r44100 -D plughw:CARD=Snowflake | tee sender.wav | ./sender_child %s", serverIP);
    sprintf(syscall, "arecord -f S16_LE -r11025 -D hw:CARD=AudioPCI | tee sender.wav | ./cb_child ");
    system(syscall);
}