#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 /* The maximum length command */

int
main(void)
{
    char * args[MAX_LINE / 2 + 1]; /* command line arguments */
    int argc = 0; /* command line argument count */
    int should_run = 1; /* flag to determine when to exit program*/

    while(should_run)
    {
        printf("osh> ");
        fflush(stdout); /* Prints prompt */

        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */
    }

    return 0;
}