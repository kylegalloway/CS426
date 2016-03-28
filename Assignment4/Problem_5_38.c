#include "Problem_5_38.h"

int available_resources = MAX_RESOURCES;
int prompt;

Monitor monitor;

int main(int argc, char const *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <number of processes>\n", argv[0]);
        exit(1);
    }
    srand(time(NULL));

    printf("Broken(0) or Unbroken(1)?\n");
    scanf("%d", &prompt);
    if (prompt == 0)
    {
        printf("You selected Broken\n");
        createProcesses(atoi(argv[1]));
    }
    else if (prompt == 1)
    {
        printf("You selected Unbroken\n");
        fixedCreateProcesses(atoi(argv[1]));
    }
    else
    {
        printf("Please use correct input!\n");
        exit(1);
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
        printf("Create Broken Process %d\n", i);
        pthread_create(&tid[i], NULL, process, NULL);
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

/* decrease available_resources by count resources */
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
    pthread_cond_signal(&monitor.cond);
    return 0;
}

int fixedCreateProcesses(int count)
{
    pthread_t tid[count];
    int i;

    monitor = mon_init();

    printf("%d processes created\n", count);
    printf("%d Available Resources\n", available_resources);
    int orig_available_resources = available_resources;
    for (i = 0; i < count; i++) {
        printf("Create Unbroken Process %d\n", i);
        pthread_create(&tid[i], NULL, fixed_process, NULL);
    }
    for (i = 0; i < count; i++) {
        pthread_join(tid[i], NULL);
    }
    printf("%d Available Resources\n", available_resources);
    printf("Should be the same as before: %d\n", orig_available_resources);

    return 0;
}

void *fixed_process(void *param)
{
    pthread_t id = pthread_self() % 10000;
    int resources_needed = (rand() % MAX_RESOURCES) + 1;

    fixed_decrease_count(resources_needed, id);
    usleep(rand() % 10);

    printf("%ld Finished processing.\n", id);
    increase_count(resources_needed);
    printf("%ld Returns %d resources.\n", id, resources_needed);
}

/* decrease available_resources by count resources */
int fixed_decrease_count(int count, pthread_t id)
{
    pthread_mutex_lock(&monitor.mutex);
    printf("%ld Needs %d resources.\n", id, count);
    while(available_resources < count)
    {
        printf("%ld Did not succeed. Count is: %d\n", id, count);
        pthread_cond_wait(&monitor.cond, &monitor.mutex);
    }
    available_resources -= count;
    printf("%ld Succeeded in getting %d resources.\n", id, count);
    pthread_cond_signal(&monitor.cond);
    pthread_mutex_unlock(&monitor.mutex);
    return 0;
}