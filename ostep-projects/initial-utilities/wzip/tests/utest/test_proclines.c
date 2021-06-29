/* test_proclines.c: unit test for proc lines
 * */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

/* read file line by line
 * handle character counts
 * write intermediate changes
 * grepeats == lrepeats
 * return ch */
char proclines (FILE *fp, u_int8_t ch, u_int32_t grepeats) {
    char *line;             /* buffer */
    size_t len = 0;
    u_int32_t lrepeats = 0; /* num local repeats of char */
    ssize_t nread;          /* number of chars read from line */

    if (grepeats > 0) {         /* char occurred in last file */
        lrepeats = grepeats;    /* not zero if char occurs accross files */
    }

    while ((nread = getline(&line, &len, fp)) != -1) {
        /* compare to previous and keep count */
        int j;
        for (j = 0; j < nread; j++) { 
            ch = line[j]; /* get the char at index zero */
            if (chcompare(ch, line[j])) { /* if the chars are the same */
                lrepeats++; /* increment local char count */
                grepeats++; /* increment global char count */
            } 
            if (!(chcompare(ch, line[j]))) {
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
    free(line);
    line = NULL;
    fclose(fp);
    return ch;
}


int main (int argc, char *argv[]) {
    u_int8_t ch;            /* storing our ascii vals (0-127) as 1 byte int */
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
        endcount = procfile(numfiles, argv, ch, grepeats);
    }
    if (endcount == 0) {
        return 0; /* should be zero */
    }
    if (endcount != 0) {
        perror("Error: program entered unknown state during procfile\n");
    }

}
