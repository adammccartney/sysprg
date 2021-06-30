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


size_t numbytes (u_int32_t num, char ch)
{
    return sizeof(num + ch);
}

/* write number of occurrences of char as 4 byte int,
 * followed by char as ASCII */
int printToken (struct token t) { /* writes <token.count><token.ch> to stdout */
    if ((t.ch < 0) || (t.ch > 127)) {
        perror("Error: token not a valid ascii character\n");
        return 1; /* shiznit ... */
    }
    size_t bytecheck;
    if ( (bytecheck = numbytes(t.count, t.ch)) == 5) { /* format correct */
        fwrite(&t.count, sizeof(u_int32_t), 1, stdout);
        printf("%c", t.ch);
    } else {
        perror("Error: compression format output not equal to five bytes\n");
        return 1; /* also shiznit ... */
    }
    return 0; /* print successful */
}

void updateToken (struct token t, u_int8_t ch) { 
    /* if you can figure out a way to check this for falsity return 1*/
    /* otherwise */
    t.count = 1;     /* set token.count back to 1 */
    t.ch = ch;       /* set new char */
}

/* boolean methods, used to test state */

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
    return tok;
}

u_int8_t peeknext (const char *line, ssize_t lsz, int current) {
    u_int8_t tok = '\0'; /* initialize on the empty ch */
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
int readlines (FILE *fp, struct token t)
{
    char *line;             /* points to buffer */
    size_t len = 0;         /* size of buffer */
    ssize_t linelen = 0;    /* number of chars read into buffer */
    int current;            /* used by scanner to indicate current read pos */
    int succ = 1;           /* failbit ... or fail 4 bytes ... whatever ! */
    u_int8_t initchar = '\0'; /* initial character */
    u_int8_t curch = '\0';  /* current char */
    u_int8_t nxtch = '\0';  /* next char */
    bool endpos = false;

    while ((linelen = getline(&line, &len, fp)) != -1) {
        int j;
        current = 0;  /* set current position to beginning of line */
        initchar = line[current];   /* read lines first character */
        updateToken(t, initchar);
        for (j = 0; j < linelen; j++) { 
            curch = advance(line, linelen, current); /* advancing the curch of nameology*/
            nxtch = peeknext(line, linelen, current);
            if ((curch == '\0') || (nxtch == '\0')) {  
                perror("Error: buffer overflow\n");
                return 1; /* shiznit */
            }
            endpos = isatend(linelen, current);
            if (endpos == false) {
                current++;
            }
            if (curch == nxtch) {
                updateToken(t, curch);
            }
            if (curch != nxtch) {
                succ = printToken(t);
                updateToken(t, nxtch);
            }
        }            
    }
    free(line);
    line = NULL;
    fclose(fp);
    return succ;
}

int procfiles (int nfiles, char *files[]) {
    FILE *fp;               /* file stream */
    int i = 1;              /* files[0] is binary file */
    struct token curtok = { .count = 0, .ch = '\0' }; /* initialize as null */

    while (i < nfiles) {
        fp = fopen(files[i], "r+");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            return 1; /* shiznit ... */
        }
        int succ = 1; /* failbit ... fail four bytes, but who's counting */
        succ = readlines(fp, curtok);
        if (succ == 0) { /* write successful */
            i++; /* get next file */
        }
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
    return 0; 
}
