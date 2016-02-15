#include <stdio.h> /* NULL, stdin, fflush, fgets, printf */
#include <stdlib.h> /* malloc, realloc, calloc, free */
#include <unistd.h> /* fork, exec* */
#include <string.h> /* strtok, strlen, strcmp, strncpy, memcpy */
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
int trim_trailing_match (char* s, char ch);

/*
    Splits the input into an array of strings based on the delimiter.
    Does not modify the input.
    Sets argc to the count of separate strings.
    Internal array is dynamic & null-terminated
 */
char** tokenize(const char* input, const char* delim, int * argc);

/*
    Takes the command array args,
    the specific index of the wanted command,
    and the run-in-background boolean.
    Runs the specified command in given foreground/background.
 */
int run_command(struct command* args, int index, int background);