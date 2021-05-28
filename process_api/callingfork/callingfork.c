/* callingfork.c: solution to homework question 1 chpt.6 ostep
 * here we see that a call to fork produces indeterminate behavior
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int x = 100; 
    int rc = fork();
    if (rc < 0) { // fork failed...fork it!
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)
        x = 900; /* change val of x in child */
        printf("(child) x: %d (pid:%d)\n", x, (int) getpid());
    } else {
        x = 700; /* change val of x in parent */
        printf("(parent %d) x: %d (pid:%d)\n",
                rc, x, (int) getpid());
    }
    return 0;
}
