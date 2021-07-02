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

void printToken (struct token t) { 
    /* write <token.count><token.ch> to stdout */
    /* write number of occurrences of char as 4 byte int,
     * followed by char as 1 byte ASCII
     * function returns 0 on success, 1 on error
     * */
    if ((t.ch < 0) || (t.ch > 127)) {
        perror("Error: token not a valid ascii character\n");
    }

    size_t bytecheck;
    if ( (bytecheck = numbytes(t)) == 5) { /* format correct */
        //printf("writing t.count: %d\n", t.count);
        fwrite(&t.count, sizeof(u_int32_t), 1, stdout);
        printf("%c", t.ch);
    } else {
        perror("Error: compression format output not equal to five bytes\n");
    }
}

struct token updateTokenCount (struct token t, char flag) {
    /* updates token.count */
    if (flag == 's') { /* flag (s)ustain, assumes token.ch is sustained */
       t.count++;
    } else if (flag == 'i') { /* initialize count */
        t.count = 1;
    } else if (flag == 'n') { /* set to zero */
        t.count = 0;
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


bool isAtEnd (ssize_t lsz, int current) { 
    if (current + 1 >= lsz - 1) { /* don't count null char at end of buf */
        return true;
    }
    return false;
}

bool isRepeat(struct token t, u_int8_t initchar) {
    if (t.ch == initchar) {
        return true;
    }
    return false;
}

bool isFirstOrigChar (int current, char lstch, struct token t) {
    /* Assumes that the token may be carrying state from another line  
     * so it performs three checks: */
    if ((current == 0)    &&     /* is head at initial read position */
        (t.count == 0)    &&     /* is the token count zero */
        (t.ch == '\0') ) {       /* is the token.ch at initial state */  
        return true;
    }
    return false;
}

bool isFirstCharRepeat (int current, char lstch, struct token t) {
    /* Assumes that we've received state from a previous line */
    if ((current == 0) &&
        (t.count > 0) &&
        (t.ch > '\0')) {
        return true;
    }
    return false;
}

bool isNewChar (int current, u_int8_t curch, u_int8_t lstch, struct token t) {
    /* Assumes that the token may be carrying state from another file
     * so it performs three checks: */
    if ((curch != t.ch)   &&
        (curch != '\0')    &&
        (current != 0)    &&     /* is head at initial read position */
        (lstch != '\0'))   {     /* is the lstch set to NULL */
        return true;
    }
    return false;
}

bool isLineRepeatChar (u_int8_t curch, u_int8_t lstch) {
    if (curch == lstch) {
        return true;
    }
    return false;
}

bool isRepeatChar (int current, u_int8_t curch, u_int8_t lstch) {
    /* Assumes that cur and nxt are equal 
     * we're not at the first read position in line */
    if ((curch == lstch)   &&  /* equality */
        (curch != '\0')   &&
        (current != 0)    &&    /* not initial char */
        (lstch != '\0')) {
        return true; 
    }
    return false;
}

bool isEndOfLine (u_int8_t cur, u_int8_t nxt) {
    /* assumes that the following conditions 
     * for end of line:   */
    if ((cur != nxt) &&  /* characters are inequal */ 
        (nxt == '\0')) { /* the next char reads null */
        return true;
    }
    return false;
}


u_int8_t peek (const char *line, int current) {
    /* current is the index currently being read */
    /* line size */
    return line[current];
}

u_int8_t peeknext (const char *line, ssize_t lsz, int current) {
    if (isAtEnd(lsz, current)) { 
        return '\0';
    }
    return line[current + 1];
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
    u_int8_t initchar = '\0'; /* initial character */
    u_int8_t curch = '\0';  /* current char */
    u_int8_t lstch = '\0';

    while ((linelen = getline(&line, &len, fp)) != -1) {
        
        current = 0;
        initchar = line[current];
        /* if there is state from last line, print it */
        if (lstch != '\0') {
            if (!(isLineRepeatChar(lstch, initchar))) {
                  printToken(t);
                  t = updateTokenCount(t, 'n');
                  t = updateTokenCh(t, '\0');
            }
        }
        for (; current < linelen; current++) {  
            curch = peek(line, current); 

            /* the order of these statements is important */ 
            if (isFirstCharRepeat(current, lstch, t)) {
                t = updateTokenCount(t, 's');
            }       

            if (isFirstOrigChar(current, lstch, t)) {
                t = updateTokenCount(t, 'i');  
                t = updateTokenCh(t, curch);
            }
             
            if (isNewChar(current, curch, lstch, t)) {
                printToken(t);
                t = updateTokenCount(t, 'i');
                t = updateTokenCh(t, curch);
            }

            if (isRepeatChar(current, curch, lstch)) {
                t = updateTokenCount(t, 's');
            }
            lstch = curch; 
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
            printToken(curtok);
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
