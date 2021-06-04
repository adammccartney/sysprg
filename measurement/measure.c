#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    /* Measure the cost of a system call
     * Back-to-back syscalls to gettimeofday happen within the microsecond
     * We need to make a bunch of syscalls in order to determine the overhead 
     */
    const size_t num_syscalls = 1000000000;
    int i;
    struct timeval begin, end, now;

    gettimeofday(&begin, NULL);
    for (i = 0; i < num_syscalls; i++) {
        gettimeofday(&now, NULL);
    }
    gettimeofday(&end, NULL);
    printf("time = %lu.%06lu\n", begin.tv_sec, begin.tv_usec);
    printf("time = %lu.%06lu\n", end.tv_sec, end.tv_usec);
    printf("Number of Calls = %lu\n", num_syscalls);

    return 0;
}
