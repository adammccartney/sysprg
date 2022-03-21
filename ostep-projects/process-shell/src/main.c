#define  _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


#define MAX_LINE_LENGTH 80
#define MAX_NUM_ARGS 10

volatile sig_atomic_t stop;

/* minor deviation from the readme, this function sets up to wait for a stop
 * signal ctlr-c 
 * NOTE: according to the signal manpage ... it's behaviour varies across unix
 * implementations and so we should be using sigaction 
 * */
void
inthand(int signum)
{
    stop = 1;
}

int
main(int argc, char *argv[])
{
    FILE *stream;
    size_t len = 0;
    ssize_t nread;
    
    signal(SIGINT, inthand);

    if ((argc != 1) && (argc != 2)) {
        printf("Usage: ./wish\n");
        printf("   or: ./wish <batchfile>\n");
        exit(EXIT_FAILURE);
    }

    char **args = malloc(sizeof(char*)); /* array to hold the args */ 
    char *line = NULL;                   /* buffer for line read */
    int i = 0;                           /* counter for args */ 
    /* Variables for parsing stdin */
    char *token = NULL;      /* buffer to hold commands */
    const char *DELIM = " "; /* use a space to delimit commands on the input */

    if (argc == 2) { /* enter batch processing mode */

        stream = fopen(args[1], "r");
        if (stream == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        while ((nread = getline(&line, &len, stream)) != -1) {
            printf("Retrieved line of length %zd:\n", nread);
            fwrite(line, nread, 1, stdout);
        }

    }


    if (argc == 1) { /* enter interactive mode */
        stream = stdin;
        printf("wish>");
        nread = getline(&line, &len, stream);
        char *str = line;

        //printf("Retrieved line of length %zd:\n", nread);
        //fwrite(line, nread, 1, stdout);

        while ( (token = strsep(&str, DELIM)) != NULL  &&  i < MAX_NUM_ARGS )
        {
            if (token[strlen(token)-1] == '\n')
                token[strlen(token)-1] = '\0';

            args[i++] = token;

            args = realloc(args, i * sizeof(char*)); /* reallocates with each arg */
        }
        args[i] = NULL; // means the last index will contain a null pointer

        /* args is now a stack of our args */
        i = 0; /* reset counter */
        while (args[i] != NULL) {
            printf("%s\n", args[i++]);
        }
    }

    free(args);
    fclose(stream); // cleanup
    exit(EXIT_SUCCESS);
}
