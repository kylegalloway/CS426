#include <pthread.h> /* pthread_* */
#include <semaphore.h> /* sem_* */
#include <stdio.h> /* NULL, printf */
#include <stdlib.h> /* srand, rand, time */
#include <time.h>   /* time */

#define CHAIRS 3
#define SCALE 2000000 /* To scale up usleep */
#define TRUE 1
#define FALSE 0

void *student(void *num);
void *TA(void *);
