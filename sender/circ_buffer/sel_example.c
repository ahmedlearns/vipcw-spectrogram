/*

  sel_example.c

  To test, vocalize the same frequency into the mic and see how quickly the changes come, 
  then change it up by doing different frequencies following by more of the same frequency, 
  to see how quickly the values popping up become 0 again. :)

*/


#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFLEN 10
  
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
  // char throwaway[1000];
  // reset(&rfds);
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  int n, i = 0;
  // int flush = 1;
  while(1) {

    flush(&rfds, &tv);
    reset(&rfds, &tv);
    while(FD_ISSET(STDIN_FILENO, &rfds)){
      n = read(fileno(stdin), buf, BUFLEN);
      printf(">>>Read %d bytes from stdin.\n", n);
    }

      // if(!flush && !FD_ISSET(STDIN_FILENO, &rfds) ){
      //   flush = 1;
      //   reset(&rfds);
      //   select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
      //   while (FD_ISSET(STDIN_FILENO, &rfds)){
      //       if (fgets(buf, BUFLEN, stdin)) {
      //           printf("Line number: %d\t DATA: ", ++i);
      //           printf("%s\n", buf);
      //       }
      //       usleep(1000);
      //   }
      // } else if (flush){
      //     reset(&rfds);
      //     retval = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
      //     if (FD_ISSET(STDIN_FILENO, &rfds)) {              
      //         fgets(throwaway,1000, stdin);
      //         printf("NOT SET: %s i is %d\n", throwaway, ++i);
      //         // if (fgets(buf, BUFLEN, stdin)) {
      //         //       printf("%s\n", buf);
      //               /* With this sleep, the loop keeps going for more iterations
      //                   My hypothesis is that when the select catches up with stdin, nothing
      //                   else is read in, meaning stdin is no longer a part of the rfds set.
      //               */
      //               // usleep(1000);
      //               // printf("%d %s\n",__LINE__ ,buf);
      //           // if (strncmp(buf, "exit", 4) == 0)
      //           //       exit(0);
      //          // }
      //      } else {
      //           flush = 0;
      //      }
      // }
  }

}
