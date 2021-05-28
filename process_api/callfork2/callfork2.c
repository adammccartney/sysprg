/* callfork2.c: solution to homework question 3 chpt.6 ostep
 * try to get the sequence of child, parent correct without calling wait()
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int initpid = (int) getpid();
    int rc = fork();
    printf("rc: %d\n", rc);
    printf("initpid: %d\n", initpid);
    if (rc < 0) { // fork failed...fork it!
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == initpid + 1) { /* initpid + 1 is the pid of child */
        printf("child says hello (pid:%d)\n", (int) getpid());
    } else {
        printf("parent says goodbye (pid:%d)\n",
                (int) getpid());
    }
    return 0;
}
