#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// The number of empty chairs outside the TA's office
#define CHAIRS 3

// Tells if the TA is helping or sleeping
enum TA_state {
  HELPING,
  SLEEPING
};
// Tells if the student is programming, waiting, or being helped
enum student_state {
  PROGRAMMING,
  WAITING,
  BEING_HELPED
};

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <# of students>\n", argv[0]);
        exit(1);
    }

    // Assign number of students
    int num_of_students = atoi(argv[1]);

}