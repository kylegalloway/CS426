#include <stdio.h> /* NULL, stdin, fflush, fgets, printf */
#include <unistd.h> /* fork, exec* */
#include <string.h> /* strtok, strlen, strcmp, strncpy */
#include <sys/types.h> /* pid_t, wait */
#include <sys/wait.h> /* wait */

#define MAX_LINE 80 /* The maximum length command */

#define MIN(a,b)(a < b ? a : b) /* Finds the min of 2 inputs */

/*
    Trims the character from the string if it is the last character.
    Returns a boolean based on whether the character was present.
 */
int
trim_trailing_match (char* s, char ch)
{
    int p = strlen(s) - 1;

    if (p > 0)
    {
        if (s[p] == ch)
        {
            s[p] = '\0';
            return 0;
        }
    }

    return 1;
}

struct command
{
    int id;
    char* tokens[MAX_LINE / 2 + 1];
    int background;
};

int
main(void)
{
    char* args[MAX_LINE / 2 + 1]; /* command line arguments */
    int argc = 0; /* Command line argument count */
    int should_run = 1; /* Flag to determine when to exit program*/
    struct command history[10];
    int command_count = 0;

    while (should_run)
    {
        printf("osh> ");
        fflush(stdout); /* Prints prompt */

        char buffer[MAX_LINE]; /* Allocates buffer of MAX_LINE size*/

        fgets(buffer, sizeof(buffer), stdin); /* Gets line of input */

        trim_trailing_match(buffer, '\n');
        int background = trim_trailing_match(buffer, '&');

        char* token = strtok(buffer," "); /* Token stores the output string */

        while (token != NULL)
        {
            args[argc++] = token; /* Adds the string to args */
            token = strtok(NULL," ");
        }

        args[argc] = token; /* Adds the string to args */

        if (strcmp(args[0], "history") != 0)
        {
            if (command_count < 10)
            {
                history[command_count].id = command_count;
                memcpy(history[command_count].tokens, args, sizeof(args));
                history[command_count].background = background;
                command_count++;
            }
            else
            {
                int i;
                for (i = 0; i < 10; ++i)
                {
                    history[i] = history[i + 1];
                }
                history[9].id = command_count;
                memcpy(history[9].tokens, args, sizeof(args));
                history[9].background = background;
                command_count++;
            }

            /* fork a child process */
            pid_t pid = fork();

            /* error occurred */
            if (pid < 0) {
                fprintf(stderr, "Fork Failed");
                return 1;
            }
            /* if child process */
            else if (pid == 0)
            {
                execvp(args[0], args);
            }
            /* else parent process*/
            else
            {
                if (background) /* If run in background */
                {
                    /* parent will wait for the child to complete */
                    wait(NULL);
                }
            }
        }
        else
        {
            int i;
            int c = MIN(command_count, 10);
            for (i = 0; i < c; ++i)
            {
                printf("%d\t%s\t%d\n", history[i].id, history[i].tokens[0], history[i].background);
            }
        }

        /* Resets args */
        memset(args, '\0', sizeof(args));

        /* Resets argc */
        argc = 0;

    }

    return 0;
}