#include <stdio.h>
#include <stdlib.h>

typedef struct Queue
{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements;
}Queue;

Queue * createQueue(int maxElements);
int Enqueue(Queue *Q, int element);
int Dequeue(Queue *Q);
int front(Queue *Q);