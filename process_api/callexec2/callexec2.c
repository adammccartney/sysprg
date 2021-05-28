/* callexex2.c: plays around with the variations of exec() to fork a process
 * and do some stuff. 
 * In this version a child process runs a script to generate a set of random
 * numbers to file
 * The parent process checks that the set contains a specific number   
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAXLINES 64 /* max number of lines to be searched */
#define MAXLEN 11   /* max length of any input line */

extern char **environ;

int binsearch(int x, int v[], int n);
int readints(const char *filename, int intptr[]);
int cmpfunc(const void * i, const void *j);

int intptr[MAXLINES]; /* main memory storage for ints */

int main(int argc, char *argv[])
{
    int initpid = (int) getpid();
    int rc = fork();
    if (rc < 0) { // fork failed...fork it!
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == initpid + 1) { // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        char *myargs[6];
        myargs[0] = strdup("python3");    /* filename of executable */
        myargs[1] = strdup("numgen.py");  /* script to run */
        myargs[2] = strdup("--ulimit=64000");    /* upper limit of rand range */
        myargs[3] = strdup("--numvals=100");         /* gen 10 signed ints */
        myargs[4] = strdup("--ofile=num.out");    /* write them here */
        myargs[5] = NULL;                  // mark end of array
        execvp(myargs[0], myargs);
        printf("this shouldn't print out");
    } else {
        //int rc_wait = wait(NULL);
        printf("hello, I am parent of %d (pid:%d)\n",
                rc, (int) getpid());
        int nints;
        char *numfile = strdup("./num.out");
        nints = readints(numfile, intptr); /* reads ints to main memory */
        if ((nints <= MAXLINES) && (nints >= 0)) {
            qsort((void*)intptr, nints, sizeof(intptr[0]), cmpfunc);
        } else {
            printf("Error with counting number of ints in file\n");
            goto error;
        }

        int luckynum = 47;
        int found = 0;

        found = binsearch(luckynum, intptr, nints);
        if (found != -1)
            printf("found %d, at index: %d\n", intptr[found], found);
        else
            printf("number not found\n");  /*todo: if not found, repeat child again*/
    }
    return 0;

error:
    return -1;
}

/* readints: gets ints from file and places them in main memory returns nints */
int readints(const char *filename, int intptr[])
{
    int nints = 0;
    FILE* fp = fopen(filename, "r");
    if (fp != NULL) { 
        int num = 0;
        while (nints <= MAXLINES - 1)
        {
            if (fscanf(fp, "%d", &num))
                intptr[nints++] = num;   
            else { 
                printf("Error parsing line\n");
                goto error;
            }
        }
    } else {
        printf("Error reading file.\n");
        goto error;
    }
    fclose (fp);

    return nints;
    
error:
    return -1;
}


/* intcmp: compare i to j */
int cmpfunc(const void *p, const void *q)
{
    int c1 = *(const int *)p;
    int c2 = *(const int *)q;
    if (c1 < c2) {
        return 0;
    }
    return 1;
}

/* binsearch: find x in v[0] <= v[1] ... <= [n-1]*/
int binsearch(int x, int v[], int n)
{
    int low, high, mid;

    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (x < v[mid])
            high = mid - 1;
        else if (x > v[mid])
            low = mid + 1;
        else /* found match */
            return mid;
    }
    return -1;
}

