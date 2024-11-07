/*
1 kb physical memory 2^10
12-bit virtual address, so 5 bits for page number and 7 bits for offset
size of page 128 bytes 2^7
each address is stored as 8 bytes unsigned long type
*/

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
        unsigned long index = (address >> 7);

        // Extract the offset, last 7 bits
        unsigned long offset = address & 0x7F; // 0x7F is 01111111 in binary

        unsigned long page_table[7] = {0x2, 0x4, 0x1, 0x7, 0x3, 0x5, 0x6};

        // Calculate the physical address, p*128 + d
        unsigned long physical_address = (page_table[index] << 7) + offset;

        fprintf(outfile, "0x%04lX\n", physical_address);
    }

    fclose(outfile);

    return 0;
}
