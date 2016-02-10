#include <stdio.h>

void
collatz(int x)
{
    if (x == 1)
    {
        printf("1\n");
    }
    else if (x % 2 == 0)
    {
        printf("%d, ", x);
        collatz(x / 2);
    }
    else
    {
        printf("%d, ", x);
        collatz(3 * x + 1);
    }
}

int
main(int argc, char *argv[])
{
    if (argc == 2)
    {
        int x = atoi(argv[1]);
        if (x > 0)
        {
            collatz(x);
        }
        else
        {
            printf("%d is not a positive integer....\n", x);
        }
    }
    else
    {
        printf("Please provide a positive integer argument (Collatz.exe 8).\n");
    }

    return 0;
}