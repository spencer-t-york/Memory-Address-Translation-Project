#include <stdio.h>

int main() {
    int pageTable[8]; // first index is reserved for OS



    return 0;
}

// be careful when accessing the array, in for loops use 1 to 7 instead of 0 to 7.
// when you get a virtual address from the mempart2.c, look up the physical table
