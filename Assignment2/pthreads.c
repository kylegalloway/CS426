#include "pthreads.h"

int count = 0;
int capacity = INIT_CAPACITY;
// int numberArray[INIT_CAPACITY];
int *numberArray;
int finalArray[INIT_CAPACITY];

int main(int argc, char *argv[])
{
    pthread_t tid_s1; /* The thread identifier */
    pthread_t tid_s2; /* The thread identifier */
    pthread_t tid_m; /* The thread identifier */
    pthread_attr_t attr_s1; /* The set of thread attributes */
    pthread_attr_t attr_s2; /* The set of thread attributes */
    pthread_attr_t attr_m; /* The set of thread attributes */
    if (argc == 2)
    {
        numberArray = malloc(INIT_CAPACITY * sizeof(numberArray));
        FILE *myFile;
        myFile = fopen(argv[1], "r");

        //read file into array
        int i, num, ch;

        if (myFile == NULL)
        {
            printf("Error Reading File\n");
            exit (0);
        }

        ch = fscanf(myFile, "%d,", &num);
        while (ch != EOF)
        {
            if (count >= capacity)
            {
                capacity *= 2;
                numberArray = realloc(numberArray, capacity * sizeof(*numberArray));
            }
            numberArray[count] = num;
            ch = fscanf(myFile, "%d,", &num);
            count++;
        }

        fclose(myFile);
        printArray(count, 1);

        /* Get the default attributes */
        pthread_attr_init(&attr_s1);
        pthread_attr_init(&attr_s2);
        pthread_attr_init(&attr_m);
        /* Create the thread */
        pthread_create(&tid_s1, &attr_s1, sort, (void *) param_left_create(count));
        pthread_create(&tid_s2, &attr_s2, sort,  (void *) param_right_create(count));
        /* Wait for the thread to exit */
        pthread_join(tid_s1, NULL);
        pthread_join(tid_s2, NULL);
        pthread_create(&tid_m, &attr_m, sort,  (void *) param_merge_create(count));
        pthread_join(tid_m, NULL);

        printArray(count, 0);
    }
    else
    {
        printf("Please provide the filename.\n");
        exit (0);
    }

    return 0;
}

parameters *param_merge_create(int length)
{
    parameters *temp = (parameters *) malloc(sizeof(parameters));
    temp->start = 0;
    temp->end = length - 1;

    return temp;
}

parameters *param_left_create(int length)
{
    parameters *temp = (parameters *) malloc(sizeof(parameters));
    temp->start = 0;
    temp->end = length / 2 - 1;

    return temp;
}

parameters *param_right_create(int length)
{
    parameters *temp = (parameters *) malloc(sizeof(parameters));
    temp->start = length / 2;
    temp->end = length - 1;

    return temp;
}

void *sort(void *param)
{
    parameters *data = (parameters *) param;
    quicksort(data->start, data->end);
    pthread_exit(0);
}

void quicksort(int l, int r)
{
   int j;

   if( l < r )
   {
        // divide and conquer
        j = partition(l, r);
        quicksort(l, j-1);
        quicksort(j+1, r);
   }
}

int partition(int l, int r)
{
   int pivot, i, j, t;
   pivot = numberArray[l];
   i = l;
   j = r+1;

   while(1)
   {
    do ++i;
    while(numberArray[i] <= pivot && i <= r);

    do --j;
    while(numberArray[j] > pivot);

    if(i >= j)
    {
        break;
    }

    t = numberArray[i];
    numberArray[i] = numberArray[j];
    numberArray[j] = t;
   }
   t = numberArray[l];
   numberArray[l] = numberArray[j];
   numberArray[j] = t;
   return j;
}

void printArray(int count, int initial)
{
    if (initial)
    {
        printf("Initial: [%d", numberArray[0]);
    }
    else
    {
        printf("Sorted:  [%d", numberArray[0]);
    }
    int i;
    for (i = 1; i < count; i++)
    {
        printf(", %d", numberArray[i]);
    }
    printf("]\n");
}