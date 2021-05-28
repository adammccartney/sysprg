/* redirection.c: from ostep section on process_api
 * here we see how to call the system to open file and redirect STDOUT there
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) { // fork failed...fork it!
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child: redirect stdout to a file 
        close(STDOUT_FILENO);
        open("./redirect.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

        // now exec "wc" ...
        char *myargs[3];
        myargs[0] = strdup("wc");  // program: "wc" (word count)
        myargs[1] = strdup("redirection.c");  // file to count
        myargs[2] = NULL;                  // mark end of array
        execvp(myargs[0], myargs);
        printf("this shouldn't print out");
    } else {  // parent goes down this path
        int rc_wait = wait(NULL);
    }
    return 0;
}
