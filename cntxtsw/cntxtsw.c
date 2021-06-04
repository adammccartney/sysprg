#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /* Measure the cost of a context switch
     * 1. run two processses on a single cpu
     * 2. set up two pipes between them (procs communicate via pipes)
     * 3. first process issues a write to the first pipe and waits for a read
     * on the second pipe.
     * 4. Upon seeing the first process waiting for something, the OS issues a
     * block for the first process and hands cpu control to the second process,
     * until it enters a wait state, then it hands control back to the first 
     * and so on...
     * we're going to use pipe() and sched_setaffinit() 
     * */


    // Set up pipe and procs
    int pipefd1[2], pipefd2[2];
    int returnstatus1, returnstatus2;
    pid_t pid;
    char pipe1writemessage[20] = "hi";
    char pipe2writemessage[20] = "hello";
    char readmessage[20];

    // set up cpu restrictions (both processes should run on same cpu)
    cpu_set_t set;
    int parentCPU, childCPU;

    parentCPU = 0;
    childCPU = 0;

    CPU_ZERO(&set);

    // set up measurement shtick
    const size_t num_syscalls = 1000;
    int i;
    struct timeval begin, end;
        
    returnstatus1 = pipe(pipefd1);
    if (returnstatus1 == -1) {
        perror("unable to create pipe 1");
        exit(EXIT_FAILURE);
    }
    
    returnstatus2 = pipe(pipefd2);
    if (returnstatus2 == -1) {
        perror("unable to create pipe 2");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork failed...fork it!");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&begin, NULL);
    if (pid != 0) { /* Parent */
        CPU_SET(parentCPU, &set);

        for (i = 0; i < num_syscalls; i++) {
            close(pipefd1[0]); /* Close unwanted pipe1 read side  */
            close(pipefd2[1]); /* Close unwanted pipe2 write side */
            printf("In parent: Writing to pipe 1 - Message is %s\n", pipe1writemessage);
            write(pipefd1[1], pipe1writemessage, sizeof(pipe1writemessage));
            read(pipefd2[0], readmessage, sizeof(readmessage));
            printf("In parent: Reading from pipe 2 - Message is %s\n", readmessage);
        }

    } else {     /* Child reads from pipe */
        CPU_SET(childCPU, &set);

        for (i = 0; i < num_syscalls; i++) {
            close(pipefd1[1]); /* Close unwanted pipe1 write side */
            close(pipefd2[0]); /* Close unwanted pipe2 read side */
            read(pipefd1[0], readmessage, sizeof(readmessage));
            printf("In child: Reading from pipe 1 - Message is %s\n", readmessage);
            printf("In child: Writing to pipe 2 - Message is %s\n", pipe2writemessage);
            write(pipefd2[1], pipe2writemessage, sizeof(pipe2writemessage));
        }
    }
        
    gettimeofday(&end, NULL);
    printf("time = %lu.%06lu\n", begin.tv_sec, begin.tv_usec);
    printf("time = %lu.%06lu\n", end.tv_sec, end.tv_usec);
    printf("Number of Calls = %lu\n", num_syscalls);
    return 0;
}
