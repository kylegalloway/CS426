#include <pthread.h> /* pthread_* */
#include <semaphore.h> /* sem_* */
#include <stdio.h> /* NULL, printf */
#include <stdlib.h> /* srand, rand, time */
#include <unistd.h> /* usleep */
#include <time.h>   /* time */

#define MAX_RESOURCES 3

int decrease_count(int count, pthread_t id);
int increase_count(int count);
int fixed_decrease_count(int count, pthread_t id);
int createProcesses(int count);
void *process(void *param);
void *fixed_process(void *param);