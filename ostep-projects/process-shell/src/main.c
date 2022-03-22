#define  _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>


#define MAX_LINE_LENGTH 80
#define MAX_NUM_ARGS 10
#define PATHLEN 4
#define DIRLEN 30

volatile sig_atomic_t stop;

void inthand(int signum);
void indexpath();

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

void indexpath()
{
    /* if we could somehow ls the contents of all of these directories, then
     * store the contents in an array of filepaths in memory, we'd have a
     * pretty nifty little index 
     * */
    char path[PATHLEN][DIRLEN] = {
                            "/usr/bin/",
                            "/home/adam/.local/bin/",
                            "/usr/bin",
                            "/bin"
                            };
    int i = 0;
    char *dirname = NULL;
    char error_message[30] = "Indexing error\n";

    for (; i < PATHLEN; i++) {
        dirname = &path[i][0];
        int rc = fork(); // create a fork
        if (rc < 0) { // fork failed ... fork it!!!    
            write(STDERR_FILENO, error_message, strlen(error_message)); 
            exit(1);
        } else if (rc == 0) { // child (new process)
            char *myargs[4];
            myargs[0] = strdup("ls");     // list 
            myargs[1] = strdup("-1");     // in one column (\n separates)
            myargs[2] = strdup(dirname);  // dirnames in path
            myargs[3] = NULL;
            // call ls on the directory path
            execvp(myargs[0], myargs);
        } else {
            int rc_wait = wait(NULL);
            printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n",
                rc, rc_wait, (int) getpid());
        }
        // consider the dirname as a prefix for the executable file

    }

}

int
main(int argc, char *argv[])
{
    FILE *stream;
    size_t len = 0;
    ssize_t nread;

    //char error_message[30] = "An error has occurred\n";
    
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

    indexpath(); // create an index of everything found on the path variable

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
