/* fkfile.c: open afile, then call fork to start another process, try to write
 * to file simultaneously 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    /* redirect stdout to file */
    close(STDOUT_FILENO);
    open("./fkfile.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

    int rc = fork();
    if (rc < 0) { /* fork failed...fork it!!! */
        fprintf(stderr, "fork failed\n");
        exit(1);

    } else if (rc == 0) { 
        // now exec "wc" ...
        char *myargs[3];
        myargs[0] = strdup("wc");  // program: "wc" (word count)
        myargs[1] = strdup("fkfile.c");  // file to count
        myargs[2] = NULL;                  // mark end of array
        execvp(myargs[0], myargs);
        printf("this shouldn't print out");
    } else {  // parent goes down this path
        // int rc_wait = wait(NULL);
        char *myargs[3];
        myargs[0] = strdup("ls");  // program: "ls" (list?)
        myargs[1] = strdup("..");  // directory whose contents to list
        myargs[2] = NULL;          // terminate myargs
        execvp(myargs[0], myargs);
    }
    return 0;
}


