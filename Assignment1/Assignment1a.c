#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
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
            execlp("/home/kyle/School/CS 426/Assignments/Collatz.exe", "Collatz.exe", argv[1], (char *)NULL);
        }
        /* parent process*/
        else
        {
            /* parent will wait for the child to complete */
            wait(NULL);
        }
    }
    else
    {
        printf("Please provide a positive integer argument (Assignment1a.exe 8).\n");
    }

    return 0;
}