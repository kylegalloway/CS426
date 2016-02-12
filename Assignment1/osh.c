#include <stdio.h> /* NULL, stdin, fflush, fgets, printf */
#include <stdlib.h> /* malloc, realloc, calloc, free */
#include <unistd.h> /* fork, exec* */
#include <string.h> /* strtok, strlen, strcmp, strncpy */
#include <sys/types.h> /* pid_t, wait */
#include <sys/wait.h> /* wait */

#define MAX_LINE 80 /* The maximum length command */
#define MAX_HISTORY 10 /* The maximum number of commands stored in history */
#define MIN(a,b)(a < b ? a : b) /* Finds the min of 2 inputs */

/* Structure to hold each individual command */
struct command
{
    int id;
    char* tokens[MAX_LINE / 2 + 1];
    int num_tokens;
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

/*
    Splits the input into an array of strings based on the delimiter.
    Does not modify the input.
    Sets argc to the count of separate strings.
    Internal array is dynamic & null-terminated
 */
char**
tokenize(const char* input, const char* delim, int * argc)
{
    char* str = strdup(input);
    int count = 0;
    int capacity = 10;
    char** result = malloc(capacity * sizeof(*result));
    char* tok=strtok(str, delim);

    while(1)
    {
        if (count >= capacity)
        {
            result = realloc(result, capacity * sizeof(*result));
            capacity *= 2;
        }

        result[count++] = tok? strdup(tok) : tok;

        if (!tok)
        {
            break;
        }

        tok=strtok(NULL, delim);
    }

    free(str);
    * argc = count;
    return result;
}

/*
    Takes the command array args,
    the specific index of the wanted command,
    and the run-in-background boolean.
    Runs the specified command in given foreground/background.
 */
int
run_command(struct command* args, int index, int background)
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
        execvp(args[index].tokens[0], args[index].tokens);
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

int
main(void)
{
    char* args[MAX_LINE / 2 + 1]; /* command line arguments */
    int argc = 0; /* Command line argument count */
    int should_run = 1; /* Flag to determine when to exit program */
    struct command history[MAX_HISTORY]; /* History array */
    int command_count = 0; /* Count of history commands */

    while (should_run)
    {
        printf("osh> ");
        fflush(stdout); /* Prints prompt */

        char buffer[MAX_LINE]; /* Allocates buffer of MAX_LINE size*/

        fgets(buffer, sizeof(buffer), stdin); /* Gets line of input */

        trim_trailing_match(buffer, '\n'); /* Trims trailing newline */
        /* Trims trailing ampersand; Sets background to success */
        int background = trim_trailing_match(buffer, '&');

        /* Copies the tokenized strings (in array form) to the args array. */
        memcpy(args, tokenize(buffer, " ", &argc), sizeof(args));

        /* If arg[0] starts with '!' */
        if(args[0][0] == '!')
        {
            char second = args[0][1];
            int second_int = second - '0';
            /* '!!' means run the previous command again. */
            if (second == '!')
            {
                /* Sets the background boolean to the current status */
                background = history[command_count % MAX_HISTORY - 1].background;
                /* Runs the command from the given index in the history with the background status. */
                run_command(history, (command_count % MAX_HISTORY) - 1, background);
            }
            /* '!N' means run the Nth command again. */
            else if (second_int > 0 && second_int < command_count)
            {
                int num = second_int - 1;
                /* Sets the background boolean to the current status */
                background = history[num].background;
                /* Runs the command from the given index in the history with the background status. */
                run_command(history, num, background);
            }
            else
            {
                printf("No such command in history.\n");
            }
        }
        /* If args[0] doesn't start with ! and isn't 'history' */
        else if (strcmp(args[0], "history") != 0)
        {
            /* Sets correct index */
            int index = command_count % MAX_HISTORY;
            /* Saves the command id */
            history[index].id = command_count;
            /* Copies contents of args to the command tokens array */
            memcpy(history[index].tokens, args, sizeof(args));
            /* Sets the command's number of tokens to argc */
            history[index].num_tokens = argc;
            /* Sets the background boolean to the current status */
            history[index].background = background;
            command_count++;
            /* Runs the command from the given index in the history with the background status. */
            run_command(history, index, background);
        }
        else
        {
            /* Sets i to the largest index used in history */
            int i = MIN(MAX_HISTORY, command_count) - 1;
            /* While i isn't negative */
            while (i >= 0)
            {
                /* Print the command id */
                printf("%d\t", history[i].id + 1);
                /* Print each token in the command's tokens array */
                int x;
                for (x = 0; x < history[i].num_tokens; ++x)
                {
                    if (history[i].tokens[x])
                    {
                        printf("%s ", history[i].tokens[x]);
                    }
                }
                printf("\n");
                --i;
            }
        }

        /* Resets args */
        memset(args, '\0', sizeof(args));

        /* Resets argc */
        argc = 0;

    }

    return 0;
}