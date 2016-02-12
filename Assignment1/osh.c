#include <stdio.h> /* NULL, stdin, fflush, fgets, printf */
#include <stdlib.h> /* malloc, realloc, calloc, free */
#include <unistd.h> /* fork, exec* */
#include <string.h> /* strtok, strlen, strcmp, strncpy */
#include <sys/types.h> /* pid_t, wait */
#include <sys/wait.h> /* wait */

#define MAX_LINE 80 /* The maximum length command */

#define MIN(a,b)(a < b ? a : b) /* Finds the min of 2 inputs */


struct command
{
    int id;
    char* tokens[MAX_LINE / 2 + 1];
    int background;
};

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

/* TODO: Description */
char**
tokenize(const char* input)
{
    char* str = strdup(input);
    int count = 0;
    int capacity = 10;
    char** result = malloc(capacity*sizeof(*result));
    char* tok=strtok(str," ");

    while(1)
    {
        if (count >= capacity)
        {
            result = realloc(result, (capacity*=2)*sizeof(*result));
        }

        result[count++] = tok? strdup(tok) : tok;

        if (!tok)
        {
            break;
        }

        tok=strtok(NULL," ");
    }

    free(str);
    return result;
}

/* TODO: Description */
int
run_command(struct command* args, int place, int background)
{
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
        execvp(args[place].tokens[0], args[place].tokens);
    }
    /* else parent process*/
    else
    {
        if (background) /* If run in background */
        {
            wait(NULL); /* parent will wait for the child to complete */
        }
    }

    return 0;
}

/* TODO: Description */
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

        memcpy(args, tokenize(buffer), sizeof(args));

        if(args[0][0] == '!')
        {
            char second = args[0][1];
            int second_int = second - '0';
            if (second == '!')
            {
                background = history[command_count % 10 - 1].background;
                run_command(history, (command_count % 10) - 1, background);
            }
            else if (second_int > 0 && second_int < 11)
            {
                int num = second_int - 1;
                background = history[num].background;
                run_command(history, num, background);
            }
            else
            {
                printf("No such command in history.\n");
            }
        }
        else if (strcmp(args[0], "history") != 0)
        {
            int spot = command_count % 10;
            history[spot].id = command_count;
            memcpy(history[spot].tokens, args, sizeof(args));
            history[spot].background = background;
            command_count++;

            run_command(history, spot, background);
        }
        else
        {
            int i = 0;
            while (i < 10 && command_count - i > 0)
            {
                printf("%d\t%s\n", MIN(10, command_count) - history[i].id, history[i].tokens[0]);
                ++i;
            }
        }

        /* Resets args */
        memset(args, '\0', sizeof(args));

        /* Resets argc */
        argc = 0;

    }

    return 0;
}