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
    char str[100];
    sprintf(str, "Hello, World\n");

    setvbuf(stdout, NULL, _IONBF, BUFFSIZE);
    char *c = str;
    while(0 != (*c)){
        putchar(*c);
        sleep(1);
        c++;
    }

    int n;
    // while( 0 < (n = read(stdin, buff, BUFFSIZE))){
    //     buff[n] = '\0';
    //     puts(buff);
    //     // printf("%s\n", buff);
    //     // int i;
    //     // for(i = 0; i < BUFFSIZE; i++){
    //     //     printf("%c", buff[i]);
    //     // }
    // }
}