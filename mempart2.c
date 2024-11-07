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
