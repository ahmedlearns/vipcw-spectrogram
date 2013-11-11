/*

	sel_main.c

*/

#include <stdlib.h>	
#include <stdio.h>	

int main(int argc, char**argv){
	char syscall[512];
	setbuf(stdout,0);
    sprintf(syscall, "arecord -f S16_LE -r22050 -D hw:CARD=AudioPCI | ./sel_example.out  > %s", argv[1]);
    system(syscall);
}