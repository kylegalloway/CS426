#include "Problem_5_38.h"

int available_resources = MAX_RESOURCES;
int prompt;
sem_t sem;

int main(int argc, char const *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <number of processes>\n", argv[0]);
        exit(1);
    }

    printf("Broken(0) or Unbroken(1)?\n");
    scanf("%d", &prompt);
    if (prompt == 0)
        printf("You selected Broken\n");
    else if (prompt == 1)
        printf("You selected Unbroken\n");
    else
    {
        printf("Please use correct input!\n");
        exit(1);
    }

    sem_init(&sem, 0, 1);
    srand(time(NULL));
    createProcesses(atoi(argv[1]));

    return 0;
}


/* decrease available_resources by count resources */
/* return 0 if sufficient resources available */
/* otherwise return -1 */
int decrease_count(int count, pthread_t id)
{
    printf("%ld Needs %d resources.\n", id, count);
    if (available_resources < count)
    {
        printf("%ld Did not succeed.\n", id);
        return -1;
    }
    else
    {
        available_resources -= count;
        printf("%ld Succeeded in getting %d resources.\n", id, count);
        return 0;
    }
}

/*increase available_resources by count */
int increase_count(int count)
{
    available_resources += count;
    return 0;
}

/* decrease available_resources by count resources */
/* return 0 if sufficient resources available */
/* otherwise return -1 */
int fixed_decrease_count(int count, pthread_t id)
{
    int run = 1;
    while(run)
    {
        /* acquire the semaphore */
        sem_wait(&sem);

        printf("%ld Needs %d resources.\n", id, count);
        /* critical section */
        if (available_resources < count)
        {
            printf("%ld Did not succeed.\n", id);
            sem_post(&sem);
        }
        else
        {
            available_resources -= count;
            printf("%ld Succeeded in getting %d resources.\n", id, count);
            /* release the semaphore */
            sem_post(&sem);
            run = 0;
        }
    }
    return 0;
}

int createProcesses(int count)
{
    pthread_t tid[count];
    int i;
    printf("%d processes created\n", count);
    printf("%d Available Resources\n", available_resources);
    int orig_available_resources = available_resources;
    for (i = 0; i < count; i++) {
        if (prompt == 0)
        {
            printf("Create Broken Process %d\n", i);
            pthread_create(&tid[i], NULL, process, NULL);
        }
        else
        {
            printf("Create Unbroken Process %d\n", i);
            pthread_create(&tid[i], NULL, fixed_process, NULL);
        }
    }
    for (i = 0; i < count; i++) {
        pthread_join(tid[i], NULL);
    }
    printf("%d Available Resources\n", available_resources);
    printf("Should be the same as before: %d\n", orig_available_resources);
}


void *process(void *param)
{
    pthread_t id = pthread_self();
    int resources_needed = (rand() % MAX_RESOURCES) + 1;

    decrease_count(resources_needed, id);
    usleep(rand() % 10);

    printf("%ld Finished processing.\n", id);
    printf("%ld Returns %d resources.\n", id, resources_needed);
    increase_count(resources_needed);
}

void *fixed_process(void *param)
{
    pthread_t id = pthread_self();
    int resources_needed = (rand() % MAX_RESOURCES) + 1;

    fixed_decrease_count(resources_needed, id);
    usleep(rand() % 10);

    printf("%ld Finished processing.\n", id);
    printf("%ld Returns %d resources.\n", id, resources_needed);
    increase_count(resources_needed);
}