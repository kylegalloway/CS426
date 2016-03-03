#include <pthread.h> /* pthread_* */
#include <semaphore.h> /* sem_* */
#include <stdio.h> /* NULL, printf */
#include <stdlib.h> /* srand, rand, time */
#include <time.h>   /* time */

#define CHAIRS 3
#define SCALE 2000000 /* To scale up usleep */

void *student(void *num);
void *TA(void *);
int chairsAvailable();
