/* vmmemman.c: short program to show some info about virtual memory management
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    long pagesize = 0;
    pagesize = sysconf(_SC_PAGESIZE);
    printf("Pagesize on this system is: %ld\n", pagesize);
    return 0;
}
