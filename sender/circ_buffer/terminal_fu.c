#include <stdio.h>
main()
{
    setbuf(stdout,0);
    while(1)  {
            sleep(1);
            putchar('x');
    }
}