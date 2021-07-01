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


size_t numbytes (struct token t) {
    /* calculates the number of bytes in the attributes of a token */
    /* assumes a token has two attributes */
    size_t mybytes = sizeof(t.count) + sizeof(t.ch);
    return mybytes;
}

int printToken (struct token t) { 
    /* write <token.count><token.ch> to stdout */
    /* write number of occurrences of char as 4 byte int,
     * followed by char as ASCII i
     * function returns 0 on success, 1 on error
     * */
    
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

struct token updateTokenCount (struct token t, char flag) {
    /* updates token.count */
    if (flag == 's') { /* flag (s)ustain, assumes token.ch is sustained */
       t.count++;
    } else if (flag == 'n') { /* flag (n)ew, assumes token.ch is new */ 
                              /* this assumption is made on having seen */
                              /* that the next char is a new token */
        t.count = 0;          /* resets t.count to initial value */
    } else if (flag == 'i') { /* we've met the initial token */
        t.count = 1;
    } else {
        perror("Error: uknown flag passed to updateTokenCount");
    }
    return t;
}

struct token updateTokenCh (struct token t, u_int8_t ch) { 
    /* updates a token's char */ 
    t.ch = ch;
    return t;
}

bool isatend (ssize_t lsz, int current) { 
    if (current >= lsz) {
        return true;
    }
    return false;
}



bool isFirstOrigChar (int current, char lstch, struct token t) {
    /* Assumes that the token may be carrying state from another file
     * so it performs three checks: */
    if ((current == 0)    &&     /* is head at initial read position */
        (lstch == '\0')   &&     /* is the lstch set to NULL */
                                 /* last char may not be NULL if the last file 
                                  * has state to maintain */
        (t.count == 0)) {        /* is the token count zero */
        return true;
    }
    return false;
}

bool isNormRepeatChar (bool endpos, u_int8_t cur, u_int8_t nxt, int current) {
    /* Assumes we might be about to overflow the buffer or that next 
     * char is new, checks for: */
    if ((endpos == false) &&  /* end of buffer */ 
        (cur == nxt)      &&  /* equality */
        (current != 0)) {     /* not initial char */
        return true; 
    }
    return false;
}

bool isNewCharInLine (bool endpos, u_int8_t cur, u_int8_t nxt) {
    /* Assumes that we might overflow the buffer 
     * or next char repeats checks: */
    if ((endpos == false) &&    /* will not try to read invalid */
        (cur != nxt)) {         /* different chars */
        return true;
    }
    return false;
}

bool isEndOfLine (bool endpos, u_int8_t cur, u_int8_t nxt) {
    if ((cur != nxt) &&
        (endpos == true)) {
        return true;
    }
    return false;
}

bool isLineRepeatChar (bool endpos, int current, char lstch) {
    /* Assume that we have received a token with state from the last line 
     * perform three checks : */
    if ((endpos == false) && /* check not overflowing buffer */
        (current == 0)    && /* head is at init read position */
        (lstch == '\0')) {   /* last char was not null */
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
                //printf("control 0\n");
                endpos = true;
            }

            if (isFirstOrigChar(current, lstch, t)) {
                //printf("control 1\n");
                t = updateTokenCount(t, 'i');  
                t = updateTokenCh(t, curch);
            }

            if (isNormRepeatChar(endpos, curch, nxtch, current)) {                
                //printf("control 2\n");
                t = updateTokenCount(t, 's');
                t = updateTokenCh(t, curch);
            }
            
            if (isNewCharInLine(endpos, curch, nxtch)) { /* new char within bounds */
                /* assumes that nxt value will be new char
                 * so write current state and reset buffer counter */
                //printf("control 3\n");
                succ = printToken(t);  /* if nxt is new char */
                t = updateTokenCount(t, 'n');
                /* does **not** take action to update token.ch */
                /* this happens in next iteration */
            } 

            if (isLineRepeatChar(endpos, current, lstch)) {  
                /* char carries over from last line */
                //printf("control 4\n");
                t = updateTokenCount(t, 's');
            }
            
            if (isEndOfLine(endpos, curch, nxtch)) { /* end of line, token has state */
                //printf("control 5\n");
                lstch = curch;      /* control for next line */
            }
            current++;
            //printf("current count: %d\n", current);
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
            curtok = updateTokenCount(curtok, 'n');  /* next token is NULL */ 
            curtok = updateTokenCh(curtok, '\0'); /* end state */
        }
        fclose(fp);
    }
    return succ; /* files processing successful */
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
