sem_t mutex = 1; //mutual exclusion to shared set of buffers
sem_t empty = N; //count of empty buffers(all empty to start)
sem_t full = 0; //count of full buffers(none full to start)

void *producer(void *param)
{
    while(1)
    {
        // Produce new resource
        wait(empty);
        wait(mutex);
        // Add resource to empty buffer
        signal(mutex);
        signal(full);
    }
}

void *consumer(void *param)
{
    while(1)
    {
        wait(full);
        wait(mutex);
        // Remove resource a from full buffer
        signal(mutex);
        signal(empty);
        // Consume resource
    }
}