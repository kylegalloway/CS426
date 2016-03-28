#include "Project3.h"


pthread_mutex_t mutex; //mutual exclusion to shared set of buffers
sem_t empty; //count of empty buffers(all empty to start)
sem_t full; //count of full buffers(none full to start)

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    /* 1. Get command line arguments argv[1], argv[2], argv[3] */
    int sleep_time, producers_count, consumers_count;
    printf("Please input the sleep time (S), # of producers (P), and # of consumers(C)?\nIn the format: S P C\n");
    scanf("%d %d %d", &sleep_time, &producers_count, &consumers_count);

    /* 2. Initialize buffer */


    /* 3. Create producer thread(s) */
    pthread_t pid[producers_count];
    pthread_t cid[consumers_count];
    int i;
    for(i = 0; i < producers_count; i++)
    {
        pthread_create(&pid[i], NULL, producer, NULL);
    }

    /* 4. Create consumer thread(s) */
    for(i = 0; i < consumers_count; i++)
    {
        pthread_create(&cid[i], NULL, consumer, NULL);
    }

    /* 5. Sleep */
    usleep(sleep_time);

    /* 6. Exit */
    exit(0);
}

void *producer(void *param)
{
    int item;
    pthread_t id = pthread_self();

    printf("Producer thread created. ID: %lu\n", id);

    while(1)
    {
        /* Sleep for a random time */
        usleep(rand() % 10);

        // Produce new resource
        buffer_item item = rand();
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        // Add resource to empty buffer
        printf("Inserting item. %d\n", item);
        insert_item(item);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);

    }
}

void *consumer(void *param)
{
    buffer_item item;

    pthread_t id = pthread_self();

    printf("Consumer thread created. ID: %lu\n", id);

    while(1)
    {
        /* Sleep for a random time */
        usleep(rand() % 10);

        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        // Remove resource a from full buffer
        item = remove_item();
        printf("Removed item. %d\n", item);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}