#include <stdio.h>
#include <stdlib.h>

#define MAX_ADDRESSES 10000

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("incorrect usage\n");
        exit(1);
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "rb"); // rb for read binary mode
    if (file == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    unsigned long addresses[MAX_ADDRESSES];
    int count = 0;

    // Read addresses from the file into array
    while (fread(&addresses[count], sizeof(unsigned long), 1, file) == 1 && count < MAX_ADDRESSES)
    {
        count++;
    }

    fclose(file);

    FILE *outfile = fopen("part1out", "w"); // open file part1out for writing
    if (outfile == NULL)
    {
        printf("Could not open output file\n");
        exit(1);
    }

    // now for each address perform the calculations
    for (int i = 0; i < count; i++)
    {
        unsigned long address = addresses[i];

        // Extract the page number, first 5 bits
        unsigned long pageNumber = (address >> 7);

        // Extract the offset, last 7 bits
        unsigned long offset = address & 0x7F; // 0x7F is 01111111 in binary

        // check every entry in page table and see if this page number exists
        // if the page number exists at an index y of the page table, then y is the frame number for the physical memory address
        // answer: physical address will be y * 128 (page size) + offset

        // if the page number doesnt exist: two scenarios
        // check if there are available entries in the page table to use, if yes then use one. increment page fault counter
        // if no available entries to use: then use LRU and replace one entry in the page table, the one that is the least recently used
        // this will count as a page fault and increment a page fault counter

        //

        // fprintf(outfile, "0x%04lX\n", physicalAddress);
    }

    fclose(outfile);

    return 0;
}

// comments for LRU:
// keep in mind: stack contains page numbers. in the page table, the indices are physical frame numbers, and the content at an index is the virtual page number
// implement using stack or linked list, implementation details will vary. opinion: array will be easier since we have to access bottom of the stack and move elements around
// when a page is accessed: two cases
// 1) if the page number doesnt exist in the stack: push it on the top
// 2) if the page number does exist in the stack: move it to the top
// what does this mean? when the page number does exist it means the page table already has an entry for it, it was just accessed again
// if the page number doesnt exist in the stack means it wasnt in the page table as well and will be added in the page table. hence add it on the top of the stack
// at any point, the element at the bottom of the stack will be the least recently used element
// so when the page table is full and an element needs to be replaced, it will be this last element that will be up for replacement
