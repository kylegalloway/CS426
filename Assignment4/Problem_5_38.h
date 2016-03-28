#include <pthread.h> /* pthread_* */
#include <semaphore.h> /* sem_* */
#include <stdio.h> /* NULL, printf */
#include <stdlib.h> /* srand, rand, time */
#include <unistd.h> /* usleep */
#include <time.h>   /* time */
// #include <sys/types.h> /* cond */

#define MAX_RESOURCES 3

typedef struct Monitor
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Monitor;

Monitor mon_init()
{
    Monitor mon;
    pthread_mutex_init(&mon.mutex, NULL);
    pthread_cond_init(&mon.cond, NULL);
    return mon;
}

int decrease_count(int count, pthread_t id);
int increase_count(int count);
int fixed_decrease_count(int count, pthread_t id);
int createProcesses(int count);
void *process(void *param);
void *fixed_process(void *param);
int fixedCreateProcesses(int count);