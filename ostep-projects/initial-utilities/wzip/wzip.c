/* wzip.c: simple project to replicate the zip file compressions tool  
 * */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096

/* Token: simple struct to hold data about char before printing */

struct token {
    u_int32_t count;
    u_int8_t ch;
};


size_t numbytes (struct token t)
{
    size_t mybytes = sizeof(t.count) + sizeof(t.ch);
    return mybytes;
}

/* write number of occurrences of char as 4 byte int,
 * followed by char as ASCII */
int printToken (struct token t) { /* writes <token.count><token.ch> to stdout */
    if ((t.ch < 0) || (t.ch > 127)) {
        perror("Error: token not a valid ascii character\n");
        return 1; /* shiznit ... */
    }
    size_t bytecheck;
    if ( (bytecheck = numbytes(t)) == 5) { /* format correct */
        //printf("writing t.count: %d\n", t.count);
        fwrite(&t.count, sizeof(u_int32_t), 1, stdout);
        printf("%c", t.ch);
    } else {
        perror("Error: compression format output not equal to five bytes\n");
        return 1; /* also shiznit ... */
    }
    return 0; /* print successful */
}

struct token updateToken (struct token t, u_int8_t ch, char flag) { 
    if (flag == 's')  { /* same token, just increment count */
        t.count++;     /* set token.count back to 1 */
    } else if (flag == 'n') {
        t.count = 1;     /* reset count */
    } else {
        perror("Error: unknown flag passed to updateToken.\n");
    }
    t.ch = ch;
    return t;
}

bool isatend (ssize_t lsz, int current) { 
    if (current >= lsz) {
        return true;
    }
    return false;
}

u_int8_t advance (const char *line, ssize_t lsz, int current) {
    u_int8_t tok = '\0';
    if (current < lsz) {
        tok = line[current];
    }
    return tok; /* returns null if current overflows buffer */
}

u_int8_t peeknext (const char *line, ssize_t lsz, int current) {
    u_int8_t tok = '\0'; /* initialize on the empty ch 
                          * returns null if next overflows buffer */
    if ((current + 1) < lsz) {
        tok = line[current];
    }
    return tok;
}

/* The following is basically a scanner
 * read file line by line
 * handle character counts
 * write intermediate changes
 * grepeats == lrepeats
 * return ch */
struct token readlines (FILE *fp, struct token t)
{
    char *line;             /* points to buffer */
    size_t len = 0;         /* size of buffer */
    ssize_t linelen = 0;    /* number of chars read into buffer */
    int current;            /* used by scanner to indicate current read pos */
    int succ = 1;           /* failbit ... or fail 4 bytes ... whatever ! */
    u_int8_t initchar = '\0'; /* initial character */
    u_int8_t curch = '\0';  /* current char */
    u_int8_t nxtch = '\0';  /* next char */
    u_int8_t lstch = '\0';
    bool endpos = false;

    while ((linelen = getline(&line, &len, fp)) != -1) {
        
        int j;
        current = 0;  /* set current position to beginning of line */
        initchar = line[current];   /* read lines first character */

        if ((lstch != initchar) && 
            (lstch != '\0')) { /* new line, new char */
            succ = printToken(t);
            printf("%c", '\n'); /* print control character null */
        } 

        for (j = 0; j < linelen; j++) { 
            
            curch = advance(line, linelen, current); /* advancing the curch of nameology*/
            nxtch = peeknext(line, linelen, current);
            endpos = isatend(linelen, current);
            //printf("t.count: %d\n", t.count);

            if (nxtch == '\0') { /* end of line attempt at buffer overflow */
                //printf("control 1\n");
                endpos = true;
            }
            if ((endpos == false) && /* this is the first original char */
                (current == 0)    &&
                (lstch == '\0')   && 
                (t.count == 0)) {
                t = updateToken(t, curch, 'n');
            }
            if ((endpos == false) &&
                (current == 0)    &&
                (lstch == '\0')   &&
                (t.count != 1)) {  /* char carries over from last line */
                t = updateToken(t, curch, 's');
            }
            if ((curch == nxtch)  &&   /* repeat char within bounds */
                (endpos == false)) {
                //printf("control 2\n");
                t = updateToken(t, curch, 's');
            }
            if ((curch != nxtch) && 
                (endpos == false)) { /* new char within bounds */
                //printf("control 3\n");
                succ = printToken(t);
                t = updateToken(t, nxtch, 'n');
            }
            if ((curch != nxtch) && 
                (endpos == true)) { /* end of line, token has state */
                //printf("control 4\n");
                lstch = curch;      /* control for next line */
            }
            
            current++;
        }            
    }
    free(line);
    line = NULL;
    return t;
}

int procfiles (int nfiles, char *files[]) {
    FILE *fp;               /* file stream */
    int i = 1;              /* files[0] is binary file */
    struct token curtok = { .count = 0, .ch = '\0' }; /* initialize as null */
    int succ = 0;

    while (i < nfiles) {
        fp = fopen(files[i], "r");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            return 1; /* shiznit ... */
        }
        curtok = readlines(fp, curtok);
        if (curtok.count != 0) { /* read successful */
            i++; /* get next file */
        }
        if ( i >= nfiles ) { /* at last file, flush state */
            succ = printToken(curtok);
            curtok = updateToken(curtok, '\0', 'n'); /* end state */
        }

        fclose(fp);
    }
    return 0; /* files processing successful */
} 

int main(int argc, char *argv[])
{
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
    int succ = 1; /* set failfourbytes */
    if (argc >= 2) { 
        succ = procfiles(argc, argv);
    }
    if (succ == 1) {
        printf("Error: failbytes are set to fail, call Sherlock!\n");
    }
    return succ; 
}
