#include <pthread.h> /* pthread_* */
#include <semaphore.h> /* sem_* */
#include <stdio.h> /* NULL, printf */
#include <stdlib.h> /* rand() */
#include <unistd.h> /* sleep(), usleep() */
#include "buffer.h"

void *producer(void *param);
void *consumer(void *param);
