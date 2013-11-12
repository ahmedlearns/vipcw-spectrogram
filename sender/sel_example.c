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

#define BUFLEN 1000

void reset(fd_set* rfds)
{
  FD_ZERO(rfds);
  FD_SET(STDIN_FILENO, rfds);
}

int main()
{
  fd_set rfds;
  struct timeval tv;
  int retval;
  char buf[BUFLEN];
  char throwaway[1000000];
  reset(&rfds);
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  int i = 0;
  int flush = 1;
  while(1) {
      if(!flush && !FD_ISSET(STDIN_FILENO, &rfds) ){
        flush = 1;
        reset(&rfds);
        select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
        while (FD_ISSET(STDIN_FILENO, &rfds)){
            if (fgets(buf, BUFLEN, stdin)) {
                printf("Line number: %d\t DATA: ", ++i);
                printf("%s\n", buf);
            }
            // usleep(1000);
        }
      } else if (flush){
          reset(&rfds);
          retval = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
          if (FD_ISSET(STDIN_FILENO, &rfds)) {              
              fgets(throwaway,1000, stdin);
              printf("NOT SET: %s i is %d\n", throwaway, ++i);
              // if (fgets(buf, BUFLEN, stdin)) {
              //       printf("%s\n", buf);
                    /* With this sleep, the loop keeps going for more iterations
                        My hypothesis is that when the select catches up with stdin, nothing
                        else is read in, meaning stdin is no longer a part of the rfds set.
                    */
                    // usleep(1000);
                    // printf("%d %s\n",__LINE__ ,buf);
                // if (strncmp(buf, "exit", 4) == 0)
                //       exit(0);
               // }
           } else {
                flush = 0;
           }
      }
  }
  return 0;
}
