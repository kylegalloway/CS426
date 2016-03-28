#include "buffer.h"

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

int head = 0;
int tail = 0;

void insert_item(buffer_item item)
{
    buffer[head++] = item;
    if (head == BUFFER_SIZE)
    {
        head = 0;
    }
}

int remove_item()
{
    int item = buffer[tail++];
    if (tail == BUFFER_SIZE)
    {
        tail = 0;
    }
    return item;
}