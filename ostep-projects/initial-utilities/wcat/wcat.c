/* wcat.c: simple project to replicate the cat program
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 255

int main(int argc, char *argv[])
{
    int error = 0;
    FILE *fp = NULL;
    /* let's get the page size on our system. we can use this as base for buf*/
    int i;

    if (argc == 1) {
        //printf("usage: wget [file1] [file2] [filen]\n");
        return 0;
    }

    if (argc > 1) {
        for (i = 1; i < argc; i++) {
            fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("Error: unable to open file %s\n", argv[i]);
                error++;
                goto exit;
            }
            char lbuf[MAXLINE]; 
            // read from file to buffer 
            while (fgets(lbuf, MAXLINE, fp)) {
                // write from buffer to stdout
                printf("%s", lbuf);
            }
            fclose(fp);
        }
        return 0;
    }

exit:
        return error;
}
