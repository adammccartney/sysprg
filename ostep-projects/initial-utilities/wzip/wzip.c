/* wzip.c: simple project to replicate the zip file compressions tool  
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 512


int main(int argc, char *argv[])
{
    FILE *fp;
    char *line;
    ssize_t nread;
    char ch = '\0';
    u_int32_t repeats = 0;

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
    if (argc >= 2) { /* read files,  */
        int i;
        u_int32_t count = 0;
        for (i = 1; i < argc; i++) {
            fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("wzip: cannot open file\n");
                return 1;
            }
            
            /* count occurrences of individual characters */
            size_t len = 0;
            //char *outstr;
            while ((nread = getline(&line, &len, fp)) != -1) {
                /* for each char, compare to previous and keep count */
                int j = 0;
                ch = line[j]; /* get the char at index count */
                for (; j < nread; j++) { /* roll through chars in line */
                    if (ch == line[j]) { /* if the chars are the same */
                        count++;
                        repeats++;
                        ch = line[j];
                    } 
                    if ( (ch != line[j]) && (ch != '\0') && (ch != EOF)){
                        if ( fwrite(&repeats, sizeof(u_int32_t), 1, stdout) != 1) {
                            return 1; /* error, we should only write 1 byte */
                        }
                        if ( (j + 1) == nread ) { /* last letter, append newline */
                            printf("%c\n", ch);
                        } else {
                            printf("%c", ch);
                        }
                        count = 0; /* reset count for new char */
                        repeats = 0; /* reset num repeats */
                    }
                }
            }
            free(line);
            line = NULL;
            fclose(fp);
        }
    }
    if ( repeats > 0 ) { /* if we made it here with repeats */
        fwrite(&repeats, sizeof(u_int32_t), 1, stdout);
        printf("%c", ch);
    }
    return 0;
}
