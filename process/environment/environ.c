/* environ.c: awayalonealastalovealongtheriverrunpasteveandadamsthroughswerveof
 * shoretobendofbaybringsusbyacommodiusvicusofrecirculationbacktohowthcastleand
 * environ.c: here are some ways to access the environment variables from
 * within a process
 * */
#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[])
{
    char **ep;
    char *shell, *sesman = NULL;

    for (ep = environ; *ep != NULL; ep++)
        printf("%s\n", *ep);

    shell = getenv("SHELL");
    sesman = getenv("SESSION_MANAGER");

    printf("Shell: %s\n", shell);
    printf("Session Manager: %s\n", sesman); 

    return 0;
}

