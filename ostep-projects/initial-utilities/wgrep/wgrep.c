/* wgrep.c: simple project to replicate the grep program
 * */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 255

int main(int argc, char *argv[])
{
    FILE *fp;
    int i;
    char sterm[MAXLINE];
    char *line;
    //char *lbuf = NULL;
    size_t len = 0;
    ssize_t nread;

    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }

    /* copy the search term from argv */
    if (strlen(argv[1]) >= MAXLINE)
            fprintf(stderr, "%s is too long!\n", argv[1]);
        else
            strcpy(sterm, argv[1]); /* copy the search term
                                       empty string is '\0' */

    if (argc == 2) { /* search stdin */
        char *buffer = (char *) malloc((strlen(line) + 1));
        while (fgets(buffer, MAXLINE, stdin) != NULL) {
            if (strstr(buffer, sterm) != NULL) { /* string found */
                printf("%s", buffer);
            }
        }
        free(buffer);
        buffer = NULL;
        return 0;
    }

    if (argc > 2) { /* search files */
        for (i = 2; i < argc; i++) {
            fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("wgrep: cannot open file\n");
                return 1;
            }
            
            /* search files */
            while ((nread = getline(&line, &len, fp)) != -1) {
                if (sterm == '\0') { /* empty string, print all lines */
                    printf("%s", line);
                } 
                
                if (strstr(line, sterm) != NULL) { /* string found */
                    printf("%s", line); /* haystack contains needle */
                }
            }
            free(line);
            line = NULL;
            fclose(fp);
        }
        return 0;
    }
}
