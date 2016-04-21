#include "assign5.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./assign5 <filename>\n");
        return 1;
    }
    /* Open the backing store. */
    FILE* backingFile = fopen(BACKING_STORE, "r");
    if (backingFile == NULL) {
        fprintf(stderr, "Error: backing store is not accessible.\n");
        return 1;
    }
    /* Open the addresses to be translated. */
    FILE* addressesFile = fopen(argv[1], "r");
    if (addressesFile == NULL) {
        fprintf(stderr, "Error: %s is not accessible.\n", argv[1]);
        return 1;
    }

    /* Prompt for size of physical memory. */
    char userInput[2] = {'0', '\0'};
    while (userInput[0] != '1' && userInput[0] != '2')
    {
        printf("Please select your physical address space:\n'1' for 32768 bytes\n'2' for 65536 bytes:\n");
        fflush(stdout);
        fgets(userInput, 2, stdin);
    }

    /* Set memory size. */
    unsigned int PHYSICAL_MEMORY_SIZE = 0;
    if (userInput[0] == '1') { PHYSICAL_MEMORY_SIZE = 32768; }
    if (userInput[0] == '2') { PHYSICAL_MEMORY_SIZE = 65536; }

    /* Keep track of memory accesses, page faults, and TLB hits. */
    int totalMemoryAccesses = 0;
    int totalPageFaults = 0;
    int totalTLBHits = 0;

    /* Allocate the physical memory to be an array of PHYSICAL_MEMORY_SIZE. */
    char physicalMemory[PHYSICAL_MEMORY_SIZE];
    /* Setup a default, invalid Page to initialize the page table. */
    const Page defaultPage = { .valid = 0 };
    /* Initialize the page table using the defaultPage. */
    Page pageTable[PAGE_TABLE_SIZE];
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i)
    {
        pageTable[i] = defaultPage;
    }

    /* Initialize an array to keep track of free frames. 0 = free. */
    int freeFrames[FRAME_COUNT];

    /* Next frame to be used. (For FIFO). */
    int nextFrame = 0;

    /* Set up translation look-aside buffer as FIFO array */
    TLBEntry TLB[TLB_SIZE];
    int TLBHead = 0;
    int CurrTLBSize = 0;

    /* Read the logical addresses and run the paging system. */
    char line[256]; /* Buffer for lines of address file. */
    while (fgets(line, sizeof(line), addressesFile) != NULL)
    {
        /* The logical address is 32 bits. */
        int logicalAddr = atoi(line);
        /* The page number is bits 15-8. */
        /* Use 16-bit mask and shift to get most significant 8 bits of that. */
        int pageNumber = (logicalAddr & 0x0000FFFF) >> 8;
        /* The page offset is bits 7-0. */
        /* Use 8-bit mask. */
        int pageOffset = logicalAddr & 0x000000FF;
        /* Checks the pageNumber & pageOffset. */
        // printf("Virtual address: %d Page number: %d Page offset: %d\n", logicalAddr, pageNumber, pageOffset);

        /* -1 indicates an unknown page number. */
        int frameNumber = -1;

        /* Check if the TLB is empty. */
        if (CurrTLBSize > 0)
        {
            /* If not empty, walk through and see if the page is in it. */
            for (int i = 0; i < CurrTLBSize; ++i)
            {
                int spot = (i + TLBHead) % TLB_SIZE;
                if (TLB[spot].valid && TLB[spot].pageNumber == pageNumber)
                {
                    ++totalTLBHits;
                    frameNumber = TLB[spot].frameNumber;
                    // printf("Page: %d Frame: %d TLB Hit: %d\n", pageNumber, frameNumber, totalTLBHits);
                }
            }
        }
        /* If the frameNumber is still -1, the TLB missed. */
        if (frameNumber == -1)
        {
            /* Check if the page is in the pageTable. */
            if (pageTable[pageNumber].valid)
            {
                frameNumber = pageTable[pageNumber].frameNumber;
            }
            /* If not, bring it into memory. */
            else
            {
                /* Record the page fault. */
                ++totalPageFaults;
                /* Jump to the next frame to be used. */
                frameNumber = nextFrame;
                // printf("Page: %d Frame: %d PageFault: %d\n", pageNumber, frameNumber, totalPageFaults);

                /* If that frame is not free, */
                if (freeFrames[frameNumber] != 0)
                {
                    /* Make all references to that frameNumber invalid. */
                    for (int i = 0; i < PAGE_TABLE_SIZE; ++i)
                    {
                        /* Find the valid reference to the frameNumber. */
                        if (pageTable[i].valid && pageTable[i].frameNumber == frameNumber)
                        {
                            /* Then make it invalid and break out of the loop. */
                            pageTable[i].valid = 0;
                            break;
                        }
                    }
                    /* Make all references to that frameNumber invalid. */
                    for (int i = 0; i < CurrTLBSize; ++i)
                    {
                        /* Find the valid reference to the frameNumber. */
                        if (TLB[i].valid && TLB[i].frameNumber == frameNumber)
                        {
                            /* Then make it invalid and break out of the loop. */
                            TLB[i].valid = 0;
                            break;
                        }
                    }
                }

                /* Copy the page from backing store into physical memory. */
                /* SEEK_SET makes fseek relative to beginning of the file. */
                fseek(backingFile, pageNumber * PAGE_SIZE, SEEK_SET);
                fread(&physicalMemory[frameNumber * FRAME_SIZE], 1, PAGE_SIZE, backingFile);

                /* Update the page table. */
                pageTable[pageNumber].frameNumber = frameNumber;
                pageTable[pageNumber].valid = 1;

                /* Mark that frame as taken. */
                freeFrames[frameNumber] = 1;

                /* Set the nextFrame to use FIFO replacement. */
                nextFrame = ++nextFrame % FRAME_COUNT;
            }

            /* Update the TLB. */
            TLBEntry newTLBEntry = {
                                .pageNumber = pageNumber,
                                .frameNumber = frameNumber,
                                .valid = 1
                            };
            TLB[TLBHead] = newTLBEntry;

            /* Update CurrTLBSize, TLBHead, and nextFrame. */
            /* Set the TLBHead to use FIFO replacement. */
            TLBHead = ++TLBHead % TLB_SIZE;
            if (CurrTLBSize < TLB_SIZE) ++CurrTLBSize;

        }
        /* Read the value from memory and print the output. */
        int physicalAddress = (frameNumber * FRAME_SIZE) + pageOffset;
        /* TODO: Value doesn't work, need to format as signed byte */
        char value = physicalMemory[physicalAddress];
        printf("Virtual address: %d Physical address: %d Value: %d\n", logicalAddr, physicalAddress, value);
        ++totalMemoryAccesses;
    }

    fclose(addressesFile);
    fclose(backingFile);

    /* Print statistics. */
    printf("Number of Translated Addresses = %d\n", totalMemoryAccesses);
    printf("Page Faults = %d\n", totalPageFaults);
    double pageFaultRate = 1.0 * totalPageFaults / totalMemoryAccesses;
    printf("Page Fault Rate = %.3f\n", pageFaultRate);
    printf("TLB Hits = %d\n", totalTLBHits);
    double TLBHitRate = 1.0 * totalTLBHits / totalMemoryAccesses;
    printf("TLB Hit Rate = %.3f\n", TLBHitRate);

    return 0;
}