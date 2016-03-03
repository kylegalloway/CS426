#include "sleeping_TA.h"

/* wait_in_chair limits the number of students in chairs outside office */
/* TA_helping provides a mutex to the TA */
/* TA_sleeping allows the TA to sleep while waiting */
/* student_being_helped makes the student to wait while being helped */
sem_t wait_in_chair, TA_helping, TA_sleeping, student_being_helped;

/* Keeps track of how many students are waiting in chairs */
// Queue *waiting_chairs = createQueue(CHAIRS);
int chairsTaken = 0;

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: %s <number of students>\n", argv[0]);
        exit(1);
    }

    int student_count = atoi(argv[1]);

    /* Gets the tid for the TA thread and all of the students */
    pthread_t TA_tid, tid[student_count];

    /* Seeds the random number generator with the current system time */
    srand(time(NULL));

    int i, students[student_count];

    /* Initialize the students array */
    for (i = 0; i < student_count; i++) {
        students[i] = i;
    }

    /* Initialize the semaphores (3rd value determines the # of shares) */
    sem_init(&wait_in_chair, 0, CHAIRS);
    sem_init(&TA_helping, 0, 1);
    sem_init(&TA_sleeping, 0, 0);
    sem_init(&student_being_helped, 0, 0);

    /* Create the TA */
    pthread_create(&TA_tid, NULL, TA, NULL);

    /* Create the students */
    for (i = 0; i < student_count; i++) {
        pthread_create(&tid[i], NULL, student, (void *) &students[i]);
    }
}

void *student(void *param) {
    int num = * (int *) param;

    while(1)
    {
        /* Program for a random amount of time */
        usleep((rand() % 10) * SCALE);

        /* Need help so try to grab a chair */
        sem_wait(&wait_in_chair);
        chairsTaken++;
        printf("Student %d is outside waiting.\n", num);

        /* Wait for the TA to be free */
        sem_wait(&TA_helping);

        /* TA is free so give up chair */
        chairsTaken--;
        sem_post(&wait_in_chair);

        /* Wake the TA */
        sem_post(&TA_sleeping);
        printf("Student %d waking the TA.\n", num);

        /* Get helped by the TA */
        sem_wait(&student_being_helped);

        /* Leave */
        sem_post(&TA_helping);
        printf("Student %d is finished with the TA.\n", num);
    }
}

void *TA(void *param) {
    /* Sleep until student arrives and wakes TA */
    printf("The TA is sleeping\n");
    sem_wait(&TA_sleeping);

    while (1) {

        if (chairsTaken == 0)
        {
            /* Sleep until student arrives and wakes TA */
            printf("The TA is sleeping\n");
            sem_wait(&TA_sleeping);
        }
        else
        {
            /* Grab student from queue */
            /* Take a random amount of time to help student */
            printf("The TA is helping a student.\n");
            usleep((rand() % 10) * SCALE / 10);
            printf("The TA has finished helping a student.\n");

            /* Tell the student to leave when they are done */
            sem_post(&student_being_helped);
        }
    }
}