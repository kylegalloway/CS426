#include <stdio.h> /* NULL, stdin, fflush, fgets, printf */
#include <unistd.h> /* fork, exec* */
#include <string.h> /* strtok, strlen */
#include <sys/types.h> /* pid_t, wait */
#include <sys/wait.h> /* wait */

#define MAX_LINE 80 /* The maximum length command */

/* Trims the newline from the string if it is the last character */
void
trim_newline (char *s)
{
    int i = strlen(s) - 1;

    if ((i > 0) && (s[i] == '\n'))
        s[i] = '\0';
}

/* Trims the newline from the string if it is the last character */
void
trim_and_char (char *s)
{
    int i = strlen(s) - 1;

    if ((i > 0) && (s[i] == '&'))
        s[i] = '\0';
}

/* Returns true if the last character is & */
int
checkbg(char *buffer)
{
    return buffer[strlen(buffer) - 1] == '&';
}

int
main(void)
{
    char * args[MAX_LINE / 2 + 1]; /* command line arguments */
    int argc = 0; /* Command line argument count */
    int should_run = 1; /* Flag to determine when to exit program*/

    while(should_run)
    {
        char buffer[MAX_LINE]; /* Allocates buffer of MAX_LINE size*/

        printf("osh> ");
        fflush(stdout); /* Prints prompt */

        fgets(buffer, sizeof(buffer), stdin); /* Gets line of input */

        trim_newline(buffer);
        int background = checkbg(buffer);
        if (background)
        {
            trim_and_char(buffer);
        }

        char * token = strtok(buffer," "); /* Token stores the output string */

        while(token != NULL)
        {
            args[argc++] = token; /* Adds the string to args */
            token = strtok(NULL," ");
        }

        args[argc] = token; /* Adds the string to args */

        /* TESTING: prints out args array */
        // int x = 0;
        // while(x < argc)
        // {
        //     printf("%s\n", args[x++]);
        // }

        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         * TODO: Check #3? I think it is backwards??
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
            return execvp(args[0], args);
        }
        /* parent process*/
        else
        {
            if (!(background)) /* If not run in background */
            {
                /* parent will wait for the child to complete */
                wait(NULL);
            }
        }

        int i = 0;
        while(i < argc)
        {
            args[i] = NULL; /* Resets args */
            i++;
        }

        argc = 0;
    }

    return 0;
}