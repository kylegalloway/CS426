#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_LINE 80 /* The maximum length command */

/* Trims the newline from the string if it is the last character */
char
*trim (char *s)
{
    int i = strlen(s)-1;

    if ((i > 0) && (s[i] == '\n'))
        s[i] = '\0';

    return s;
}

// struct command
// {
//     int id;
//     char **args;
// };

int
main()
{
    // struct command hist_arr[10]; /* History array */
    // int com_p = 0;  /* History array pointer */
    // int comc = 0; /* Count of commands in history */
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int argc = 0; /* command line argument count */
    int should_run = 1; /* flag to determine when to exit program*/

    while(should_run)
    {
        char buf[MAX_LINE]; /* Allocates buffer of MAX_LINE size*/

        printf("osh> ");
        fflush(stdout); /* Prints prompt */

        fgets(buf, sizeof(buf), stdin);/* Gets line of input */

        char * tok; /* Token to store each string */
        tok = strtok(trim(buf)," ");

        while(tok != NULL)
        {
            args[argc++] = tok; /* Adds the string to args */
            tok = strtok(NULL, " ");
        }

        args[argc++] = '\0';

        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */

        /* fork a child process */
        pid_t pid = fork();

        /* error occurred */
        if (pid < 0) {
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        /* child process */
        else if (pid == 0)
        {
            execvp(args[0], args[1-...]);
        }
        /* parent process*/
        else
        {
            /* parent will wait for the child to complete */
            wait(NULL);
        }

        // hist_arr[com_p].args = args;
        // hist_arr[com_p].id = comc;

        // int i = 0;
        // while(i < argc)
        // {
        //     args[i] = '\0'; /* reset args array */
        //     ++i;
        // }

        // argc = 0; /* reset argument count */
        // comc++; /* increment command counter */
        // if (com_p <= 9)
        //     com_p++; /* increment command pointer */
        // else
        //     com_p = 0;

        //  /* TESTING: Prints what is currently in history  */
        // int j = 0;
        // while(j < com_p)
        // {
        //     printf("%d  %s", hist_arr[j].id, hist_arr[j].args[0]);
        //     int k = 1;
        //     while(hist_arr[j].args[k] != '\0')
        //     {
        //         printf("%s", hist_arr[j].args[k]);
        //     }
        //     printf("\n");

        //     ++j;
        // }
        // printf("%d:%d\n", comc, com_p);
    }

    return 0;
}