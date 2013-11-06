#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BUFFSIZE 20

int main(void) {
    char buff[50];
    int n;
    while( 0 < (n = read(fileno(stdin), buff, BUFFSIZE))){
        buff[n] = '\0';
        puts(buff);
        // int i;
        // for(i = 0; i < BUFFSIZE; i++){
        //     printf("%c", buff[i]);
        // }
    }
}