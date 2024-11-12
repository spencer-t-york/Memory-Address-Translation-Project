#include <stdio.h>
#include <stdlib.h>
#include "phyframe.h"
#include "pagetable.h"

#define MAX_ADDRESSES 10000

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("incorrect usage\n");
        exit(1);
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "rb"); // rb for read binary mode
    if (file == NULL) {
        printf("File not found\n");
        exit(1);
    }

    unsigned long addresses[MAX_ADDRESSES];
    int count = 0;

    // Read addresses from the file into array
    while (fread(&addresses[count], sizeof(unsigned long), 1, file) == 1 && count < MAX_ADDRESSES) {
        count++;
    }
    fclose(file);

    FILE *outfile = fopen("part2out", "w"); // open file part2out for writing
    if (outfile == NULL) {
        printf("Could not open output file\n");
        exit(1);
    }


    int pageTable[8] = {-1, -1, -1, -1, -1, -1, -1, -1}; // maps virtual pages to physical frames
    Stack LRUstack;
    initializeStack(&LRUstack);                          // initilize LRU Stack
    int pageFaultCounter = 0;

    for (int i = 0; i < count; i++) {
        unsigned long address = addresses[i];
        unsigned long virtualPageNumber = (address >> 7); // Extract the page number, first 5 bits
        unsigned long offset = address & 0x7F; // Extract the offset, last 7 bits (0x7F is 01111111 in binary)

        int physicalFrameNumber = getPhysicalFrameNumber(pageTable, &LRUstack, virtualPageNumber, &pageFaultCounter);
        unsigned long physical_address = physicalFrameNumber * 128 + offset;

        fprintf(outfile, "0x%04lX\n", physical_address);
    }

    printf("Page Fault Count: %d\n", pageFaultCounter);
    fclose(outfile);
    return 0;
}
