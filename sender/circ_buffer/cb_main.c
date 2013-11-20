/*

	cb_main.c

*/

#include <stdlib.h>	
#include <stdio.h>	

int main(){
	char syscall[512];
    // sprintf(syscall, "arecord -f S16_LE -r11025 -D hw:CARD=AudioPCI | tee sender.wav | ./cb_child ");
    sprintf(syscall, "arecord -f S16_LE -r512 -D hw:CARD=AudioPCI | ./cb_child ");
    system(syscall);
}