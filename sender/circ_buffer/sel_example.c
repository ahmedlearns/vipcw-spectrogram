/*

  sel_example.c

  To test, vocalize the same frequency into the mic and see how quickly the changes come, 
  then change it up by doing different frequencies following by more of the same frequency, 
  to see how quickly the values popping up become 0 again. :)

  http://stackoverflow.com/questions/717572/how-do-you-do-non-blocking-console-i-o-on-linux-in-c?lq=1
  http://stackoverflow.com/questions/315111/cross-platform-linux-win32-nonblocking-c-io-on-stdin-stdout-stderr?rq=1
  http://stackoverflow.com/questions/6107038/reading-from-stdin-using-select-in-a-nonblocking-way

*/


#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#define BUFLEN 1
  
char buf[BUFLEN];

void reset(fd_set* rfds, struct timeval* tv)
{
  FD_ZERO(rfds);
  FD_SET(STDIN_FILENO, rfds);
  select(STDIN_FILENO + 1, rfds, NULL, NULL, tv);
}

void flush(fd_set* rfds, struct timeval* tv){
  int n, count = 0;
  // Put stdin in the set
  reset(rfds, tv);
  // Read until it is removed from the set
  while(FD_ISSET(STDIN_FILENO, rfds)){
    n = read(fileno(stdin), buf, BUFLEN);
    count += n;
    printf("In flush(): flushed %d bytes...\n", n);
  }
  printf("STDIN is empty, flushed %d bytes.\n", count);
}

/*
 *  1) flush stdin
 *  2) read N bytes from stdin
 *  3) sleep for tv seconds
 *  4) repeat
 *
 */
int main()
{
  fd_set rfds;
  struct timeval tv;
  int retval;
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  // setbuf(stdin, NULL);
  // fcntl(STDIN_FILENO, F_SETFL, fcntl(0, GETFL) | O_NONBLOCK);

  int n, i = 0;
  // int empty = 0;
  while(1) {

    // if (!empty){
        reset(&rfds, &tv);
        while(FD_ISSET(STDIN_FILENO, &rfds)){
            n = read(fileno(stdin), buf, BUFLEN);
            printf(">>>Read %d byte(s) from stdin: %c\n", n, buf[0]);
            sleep(5);

            // flush(&rfds, &tv);
            // __fpurge(stdin);
            
            /*int c = getchar();
            while(c != EOF && c != '\n')
                c = getchar();*/
        }
    //     }
    //     empty = 1;
    // } else {
    //     printf("Going to flush stdin.\n");
    //     flush(&rfds, &tv);
    //     empty = 0;
    // }
  }

}
