#include <pthread.h> /* pthread_* */
#include <stdio.h> /* NULL, stdin, fflush, fgets, printf */
#include <stdlib.h> /* malloc, realloc, calloc, free */
#include <string.h> /* strtok, strlen, strcmp, strncpy, memcpy */

#define INIT_CAPACITY 10 /* The initial capacity */

/* structure for passing data to threads */
typedef struct
{
    int start;
    int end;
} parameters;

void *sort(void *param);
parameters *param_merge_create(int length);
parameters *param_left_create(int length);
parameters *param_right_create(int length);
void quicksort(int l, int r);
int partition(int l, int r);
void printArray(int count, int initial);