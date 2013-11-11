#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFLEN 1000

int main()
{
  fd_set rfds;
  struct timeval tv;
  int retval;
  char buf[BUFLEN];
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds);
  tv.tv_sec = 2;
  tv.tv_usec = 0;
  while(1) {
      retval = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
      if (FD_ISSET(STDIN_FILENO, &rfds)) {
          printf("here\n");
          if (fgets(buf,BUFLEN, stdin)) {
                printf("%d %s\n",__LINE__ ,buf);
            if (strncmp(buf, "exit", 4) == 0)
                  exit(0);
           }
           // printf("\nEnter command: ");
      }
  }
  return 0;
}
