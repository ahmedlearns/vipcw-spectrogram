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
    // char str[100];
    // sprintf(str, "Hello, World\n");

    // select() !!!!!!!!!!!!!!!!!!!!!!
    // if (select(n)) read(n) else it would block.

// Use termios to turn off line buffering
    struct termios term;
    tcgetattr(fileno(stdin), &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(fileno(stdin), TCSANOW, &term);
    setbuf(stdin, NULL);

    // setvbuf(stdin, NULL, _IONBF, BUFFSIZE);
    // setvbuf(stdout, NULL, _IONBF, BUFFSIZE);
    // char *c = str;
    // while(0 != (*c)){
    //     putchar(*c);
    //     sleep(1);
    //     c++;
    // }

    int n;
    while( !(feof(stdin)) ){
        // n = fgets(buff, 1, stdin);
        // buff[n] = '\0';
        // puts(buff);
        putchar(getchar());

        // printf("%s\n", buff);
        // int i;
        // for(i = 0; i < BUFFSIZE; i++){
        //     printf("%c", buff[i]);
        // }
    }
}