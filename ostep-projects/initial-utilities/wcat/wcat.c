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
    //size_t pgsize = getpagesize();
    int i;
    //size_t fsize; 

    // create a buffer variable to temporarily store the files being passed
    //char *buf;
    //size_t bufsize;
    
    if (argc == 1) {
        printf("usage: wget [file1] [file2] [filen]\n");
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
            // get the file size
            //fseek(fp, 0L, SEEK_END);
            //fsize = ftell(fp);
            //bufsize = pgsize - fsize; 
            //rewind(fp); 
            //buf = malloc(bufsize); 
            // simplecat(fp); 
            char lbuf[MAXLINE]; 
            // read from file to buffer 
            if ( fgets(lbuf, MAXLINE, fp) != NULL) {
                // write from buffer to stdout
                //printf("%s", lbuf);
                puts(lbuf);
            }
            //free(buf);
            fclose(fp);
        }
        return 0;
    }

exit:
        return error;
}
