#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/*
 * pipong is a program that uses UNIX system calls to “ping-pong” a byte between
 * two processes over a pair of pipes, one for each direction. Measure the
 * program’s performance, in ex- changes per second.
 *
 * it's an exercise from:
 * <https://pdos.csail.mit.edu/6.1810/2024/xv6/book-riscv-rev4.pdf>
 * */


//struct pp_child {
//    int out_fd[2];
//    int in[2];
//    int result_pipe_fd[2];
//};

/*
 * this is one side of the pingpong game, it 
 * out -> end of the pipe to write output (fd)
 * */
void timed_pingpong(float delayInSeconds, int in_fd, int out_fd, int result_fd) {
    time_t startTime;
    time_t now;
    float elapsedTime = 0;
    float setTime = delayInSeconds;

    char c;
    int count = 0;

    time(&startTime);
    while (elapsedTime < setTime) {
        if (read(in_fd, &c, 1) != 1)  /* read 1 byte from in_fd (ping) */
            break;
        write(out_fd, &c, 1);         /* write 1 byte to out_fd (pong) */
        count++;
        now = time(NULL);
        elapsedTime = difftime(now, startTime);
    }
    // write the total number of bytes written
    write(result_fd, &count, sizeof(count));
    close(result_fd);
}

int main(int argc, char *argv[])
{
    pid_t c1, c2;

    // pipe descriptors
    int p2c[2];  /* parent to child */
    int up[2];
    int down[2];
    int r1[2], r2[2];

    if ((pipe(up) == -1) || (pipe(down)) ||
        (pipe(r1) == -1) || (pipe(r2))) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // internal buffers for children
    char buf1, buf2;

    // counters for checking how many bytes are written
    int counter = 0;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <string> <runtime (sec)>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* could add check here to make sure arv[1] is only a single byte */
    /* but maybe it's easier to always just play with one byte */

    float delay_sec = atof(argv[2]);

    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    /* spawn two child processes */
    c1 = fork();
    if (c1 == -1) {
        perror("fork");
    }
    if (c1 == 0) {
        // close fds that we don't need here
        close(down[1]);
        close(up[0]);
        close(r1[0]);
        // read down stdin, write up stdout
        timed_pingpong(delay_sec, down[0], up[1], r1[1]);
        exit(0);
    }


    c2 = fork();
    if (c2 == -1) {
        perror("fork");
    }
    if (c2 == 0) {
        // close unneeded fds
        close(up[1]);
        close(down[0]);
        close(r2[0]);
        // read up stdin, write down stdout
        timed_pingpong(delay_sec, up[0], down[1], r2[1]);
        exit(0);
    }

    /* Parent writes the initial byte from argv[1] to up_pipe */
    write(up[1], argv[1], 1);

    long c1_count = 0, c2_count = 0;
    read(r1[0], &c1_count, sizeof(c1_count));
    read(r2[0], &c2_count, sizeof(c2_count));

    waitpid(c1, NULL, 0);
    waitpid(c2, NULL, 0);

    printf("Child 1: %ld, child 2: %ld bytes in %.2f seconds\n", c1_count, c2_count, delay_sec);
    exit(EXIT_SUCCESS);
}
