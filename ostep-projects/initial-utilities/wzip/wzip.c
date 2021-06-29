/* wzip.c: simple project to replicate the zip file compressions tool  
 * */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

bool issinglefile (int argc) 
{
    if (argc == 2) { 
        return true;
    }  
    return false;
}

bool ismultifile (int argc)
{
    if (argc > 2) { 
        return true;
    } 
    return false;
}

bool chcompare (u_int8_t a, u_int8_t b)
{
    if (a == b) { 
        return true;
    } 
    return false;
}

bool isendlbuf (char ch)
{
    /* end of line buffer */
    if (ch == '\0') { 
        return true;
    }
    return false;
}

bool isnewline (char ch)
{
    /* buffer contains newline */
    if (ch == '\n') { 
        return true;
    }
    return false;
}

bool isendof (char ch)
{
    /* buffer contains end of file */
    if (ch == EOF) { 
        return true;
    }
    return false;
}

bool isnewchar (char ch)
{
    if ( (!(isnewline(ch))) &&
         (!(isendlbuf(ch))) &&
         (!(isendof(ch))) ) { 
        return true;
    }
    return false;
}

size_t numbytes (u_int32_t num, char ch)
{
    return sizeof(num + ch);
}

/* write number of occurrences of char as 4 byte int,
 * followed by char as ASCII */
int writenumch (u_int32_t num, u_int8_t ch) 
{
    if ((ch < 0) || (ch > 127)) {
       perror("Error: ascii range exceeded.\n");
       return 1;
    } 
    size_t bytecheck;
    if ( (bytecheck = numbytes(num, ch)) == 5) {
        fwrite(&num, sizeof(u_int32_t), 1, stdout);
        printf("%c", ch);
    } else {
        perror("Error: number of bytes output is not five\n"); 
        printf("Trying to write %zu bytes\n", bytecheck);
        return 1;
    }
    num = 0; /* resets count to zero if write successful  */
    return num;
}

/* read file line by line
 * handle character counts
 * write intermediate changes
 * grepeats == lrepeats
 * return ch */
char proclines (FILE *fp, u_int8_t ch, u_int32_t grepeats)
{
    char *line;             /* buffer */
    size_t len = 0;
    u_int32_t lrepeats = 0; /* num local repeats of char */
    ssize_t nread;          /* number of chars read from line */
    u_int8_t tmp = 0;
    
    if (grepeats > 0) {         /* char occurred in last file */
        lrepeats = grepeats;    /* not zero if char occurs accross files */
    }

    while ((nread = getline(&line, &len, fp)) != -1) {
        /* compare to previous and keep count */
        int j;
        for (j = 0; j < nread; j++) { 
            ch = line[j]; /* get the char at index zero */
            tmp = (u_int8_t) line[j]; /* cast the char from file to ascii */
            printf("tmp: %hhu\n", tmp);
            if ((tmp < 0) || (tmp > 127)) {
                perror("Error: file contains non-ascii characters.\n");
                ch = 0; /* set char to null and exit */
                goto clear;
            }
            if (chcompare(ch, tmp)) { /* if the chars are the same */
                lrepeats++; /* increment local char count */
                grepeats++; /* increment global char count */
            } 
            if (!(chcompare(ch, tmp))) {
                if (isnewchar(line[j])) { /* newchar:
                                             write output and reset count */
                    if ((grepeats = writenumch(lrepeats, ch) != 1)) {
                            /* write successful, count value now zero */
                            lrepeats = grepeats;
                        } 
                } else if (isnewline(line[j])) { /* newline */
                    j++; /* move past newline */
                    ch = line[j];     /* continue with for loop */ 
                } else if (isendlbuf(line[j])) { /* endlbuf */
                    grepeats = lrepeats; /* continue count next linebuf */
                } else if (isendof(line[j])) { /* EOF */
                    grepeats = lrepeats; /* continue count next file */
                } else {
                    perror("Error: unknown state for chcompare\n");
                    return 1;
                }
            }
        }
    }

clear:
    free(line);
    line = NULL;
    fclose(fp);
    return ch;
}

int procfile (int nfiles, char *argv[], u_int8_t ch, u_int32_t grepeats)
{
    FILE *fp;               /* file stream */
    int i = 1;
    u_int8_t tch = 0;
    int finalcount = 0;
    while (i < nfiles) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            return 1;
        }
        if (proclines(fp, ch, grepeats)) { 
            printf("Attempting to process lines\n");
            i++; /* get next file */
        }
    }
    if (grepeats != 0) {/* state in last file, last line */
        if ( (finalcount = writenumch(grepeats, tch)) != 1) {
            /* write successful, end program*/
            return finalcount;
        }
    }
    return finalcount;
} 

int main(int argc, char *argv[])
{
    u_int8_t ch = 0;            /* storing our ascii vals (0-127) as 1 byte int */
    u_int32_t grepeats = 0; /* keeps count of occurrences of char globally */
    int endcount = 0;
    int numfiles;

    /* case that there are no files to search */
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    /* if wzip encounters files
     * it uses run length encoding to compress the data 
     * RLE functions as follows:
     * when you encounter **n** characters of the same type in a row, 
     * the compression tool (**wzip**) will turn that into the number **n**
     * and a single instance of the character.
     * */
    numfiles = argc; 
    if (argc >= 2) { 
        printf("Calling procfile with %d\n", numfiles);
        endcount = procfile(numfiles, argv, ch, grepeats);
    }
    if (endcount == 0) {
        return 0; /* should be zero */
    }
    if (endcount != 0) {
        perror("Error: program entered unknown state during procfile\n");
    }
}
