int trim_trailing_newline_and_ampersand (char * s);
int tokenize(char ** args, int * argc, char * buffer);
int add_to_history(char * buffer, char ** history, int * command_count);
int history_command(char * buffer);


int
main(void)
{
    char * history[10];
    int command_count = 0;
    char * args[MAX_LINE / 2 + 1];
    int argc = 0;
    int should_run = 1;

    while (should_run)
    {
        printf("osh> ");
        fflush(stdout);

        char buffer[MAX_LINE];

        fgets(buffer, sizeof(buffer), stdin);

        char * first = buffer;
        if (history_command(first))
        {
            printf("%d\n", command_count);
            if (command_count > 10)
            {
                int c;
                for (c = 10; c > 0; --c)
                {
                    printf("%d\t%s\n", command_count - 10 + c, history[c]);
                }
            }
            else
            {
                int c;
                for (c = command_count; c > 0; --c)
                {
                    printf("%d\t%s\n", command_count, history[c]);
                }
            }
        }
        else
        {

            printf("%d\n", add_to_history(buffer, history, command_count));
            int background = tokenize(args, &argc, buffer);
            pid_t pid = fork();

            if (pid < 0) {
                fprintf(stderr, "Fork Failed");
                return 1;
            }
            else if (pid == 0)
            {
                return execvp(args[0], args);
            }
            else
            {
                if (background)
                {
                    wait(NULL);
                }
            }

            int i = 0;
            while (i < argc)
            {
                args[i] = NULL;
                i++;
            }
            argc = 0;
        }
    }

    return 0;
}