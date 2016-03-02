#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;

/* decrease available_resources by count resources */
/* return 0 if sufficient resources available */
/* otherwise return -1 */
int decrease_count(int count)
{
    if (available_resources < count)
        return -1;
    else
    {
        available_resources -= count;
        return 0;
    }
}

 /*increase available_resources by count */
 int increase_count(int count)
 {
    available_resources += count;
    return 0;
 }

 printf("(a) The race condition is that both modifiers can access available_resources at the same time.\n");
 printf("(b) The race condition appears in both the decrease_count and increase_count.\n");

printf("
#include <semaphore.h>
#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
sem_t sem;

/* Create the semaphore and initialize it to 1 */
sem_init(&sem, 0, 1);

/* decrease available_resources by count resources */
/* return 0 if sufficient resources available */
/* otherwise return -1 */
int decrease_count(int count)
{
    if (available_resources < count)
        return -1;
    else
    {
        /* acquire the semaphore */
        sem_wait(&sem);

        /* critical section */
        available_resources -= count;

        /* release the semaphore */
        sem_post(&sem);

        return 0;
    }
}

 /*increase available_resources by count */
 int increase_count(int count)
 {
    /* acquire the semaphore */
    sem_wait(&sem);

    /* critical section */
    available_resources += count;

    /* release the semaphore */
    sem_post(&sem);

    return 0;
 }\n")