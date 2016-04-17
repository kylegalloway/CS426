#include <stdio.h> /* NULL, printf */
#include <stdlib.h> /* srand, rand, time */

// #define VIRTUAL_ADDRESS_SPACE 65536
#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define TLB_SIZE 16
#define FRAME_SIZE 256
#define FRAME_COUNT 256
#define PHYSICAL_MEMORY_SIZE 65536

#define BACKING_STORE "BACKING_STORE.bin"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./assign5 <filename>\n");
        return 1;
    }
    /* Open the backing store. */
    FILE* swapFile = fopen(BACKING_STORE, "r");
    if (swapFile == NULL) {
        fprintf(stderr, "Error: backing store is not accessible.\n");
        return 2;
    }
    /* Open the addresses to be translated. */
    FILE* addressFile = fopen(argv[1], "r");
    if (addressFile == NULL) {
        fprintf(stderr, "Error: %s is not accessible.\n", argv[1]);
        return 3;
    }
    return 0;
}