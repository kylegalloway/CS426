#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHAIRS 3

// defines how quickly the simulation proceeds
#define TIMESCALE 1000000
// defines how long the simulation lasts
#define SIM_DURATION 10

void * student(void *param);
void * teacher(void *param);

enum student_state {
  PROGRAMMING,
  WAITING,
  BEING_HELPED
};

enum teacher_state {
  HELPING,
  SLEEPING
};

int run = 1;
// teacher sem flags the teacher down
sem_t teacher_sem;
// student sem lets a student know that they're done being helped
sem_t student_sem;
// chair sem represents the chairs outside
sem_t chair_sem;
// ta_avail_sem is a mutex for the TA's help
sem_t ta_avail_sem;

void * student(void *param) {
  // keep track of the state of the student
  enum student_state state = PROGRAMMING;
  // acquire a semaphor
  sem_wait(&student_sem);
  // cast the param to an int; this is an id number
  int student_number = *((int*)param);
  int chair_avail = 0;
  while(run) {
    switch(state) {
      case PROGRAMMING:
        // 1.) Program for a period of time
        // wait for a period of time
        printf("student %d is programming\n", student_number);
        fflush(stdin);
        usleep((rand() % 10) * TIMESCALE);
        state = WAITING;
        break;
      case WAITING:
        // post to the teacher's sem, to alert that we want help
        printf("student %d needs help\n", student_number);
        fflush(stdin);

        sem_getvalue(&chair_sem, &chair_avail);
        if(chair_avail) {
        // if there is a chair available
          // alert the TA that help is required
          sem_wait(&chair_sem);
          sem_post(&teacher_sem);
          printf("student %d is waiting for help\n", student_number);
          // wait on the TA's availability
          sem_wait(&ta_avail_sem);
          printf("student %d is receiving help\n", student_number);
          // wait for the teacher to give back the sem after finished helping
          sem_wait(&student_sem);
          // then post that the TA is now available
          sem_post(&ta_avail_sem);
          // and post a chair
          sem_post(&chair_sem);
        } else {
        // if there is no chair available, go back to programming
          printf("no chairs available for student %d, back to programming\n", student_number);
        }
        state = PROGRAMMING;
        break;
      case BEING_HELPED:
        state = PROGRAMMING;
        break;
    }

    // just as a test spew
    // printf("student %d says hi\n", student_count);
    // sleep(1);
  }
  return NULL;
}

void * teacher(void *param) {
  while(run) {
    // 1.) When no students waiting, nap
    sem_wait(&teacher_sem);
    // 2.) When students waiting, help the student
    // printf("TA is helping a student\n");
    // fflush(stdin);
    usleep((rand() % 10) * (TIMESCALE/10));
    // printf("TA is done helping a student\n");
    // fflush(stdin);
    sem_post(&student_sem);
    // sleep(1);
  }
  return NULL;
}

int main(int argc, char * argv[]) {
  if(argc != 2) {
    printf("Usage: %s student_count\n", argv[0]);
    exit(1);
  }
  int student_count = atoi(argv[1]);

  // initialize the random number generator
  srand(time(NULL));

  // initialize the teacher signaling semaphor
  sem_init(&teacher_sem, 0, 0);
  sem_init(&student_sem, 0, student_count);
  sem_init(&chair_sem, 0, 4);
  sem_init(&ta_avail_sem, 0, 1);

  // teacher thread and attributes
  pthread_t teacher_thread;
  pthread_attr_t teacher_attr;

  // student threads and attributes, and number
  pthread_t students_thread[student_count];
  pthread_attr_t students_attr[student_count];
  int students_id[student_count];

  // initialize the teacher thread
  pthread_attr_init(&teacher_attr);
  pthread_create(&teacher_thread, &teacher_attr, teacher, NULL);

  // initialize the student threads
  int i;
  for(i = 0; i < student_count; i++) {
    pthread_attr_init(&students_attr[i]);
    students_id[i] = i;
    pthread_create(&students_thread[i], &students_attr[i], student, &students_id[i]);
  }

  // let the threads run
  sleep(SIM_DURATION);
  // then tell them to pack up close
  run = 0;

  // join all the threads back to the main thread
  /*
  pthread_join(teacher_thread, NULL);
  for(i = 0; i < student_count; i++) {
    pthread_join(students_thread[i], NULL);
  }

  // and close the semaphores
  sem_destroy(&student_sem);
  sem_destroy(&teacher_sem);
  sem_destroy(&chair_sem);
  sem_destroy(&ta_avail_sem);

  * actually, instead, let the OS clean up the mess.
  */

  // successful execution!
  return 0;
}
