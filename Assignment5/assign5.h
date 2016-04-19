#include <stdio.h> /* NULL, printf */
#include <stdlib.h> /* srand, rand, time */

#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define TLB_SIZE 16
#define FRAME_SIZE 256
#define FRAME_COUNT (MEMORY_SIZE / PAGE_SIZE)
#define PHYSICAL_MEMORY_SIZE 65536

#define BACKING_STORE "BACKING_STORE.bin"

typedef struct {
  int frameNumber;
  int valid;
} Page;

typedef struct {
  int pageNumber;
  int frameNumber;
  int valid;
} TLBEntry;