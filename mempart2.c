#include <stdio.h>
#include <stdlib.h>

#define MAX_ADDRESSES 10000

// first frame is reserved for the OS
// simple page allocation scheme: start from 1,2,3
// if frame is available, then allocate from available
// if not available, use LRU page replacement policy to choose a frame to be replaced
#include <stdlib.h>

typedef struct Stack {
    int pageNumbers[7]; // as there are only 7 physical frames to use, it is easier to use 7 here as the size of the stack
    int top;
} Stack;


void initializeStack(Stack *stack) {
    stack->top = -1;
}

void moveToTop(Stack *stack, int index) {
    int pageNumber = stack->pageNumbers[index];
    for (int i = index; i < stack->top; i++) {
        stack->pageNumbers[i] = stack->pageNumbers[i + 1];
    }
    stack->pageNumbers[stack->top] = pageNumber; // move accessed page number to the top
}

void pushPage(Stack *stack, int pageNumber) {
    if (stack->top < 6) { // stack is not full and we can just put the page number at the top
        stack->pageNumbers[++stack->top] = pageNumber;
    }
    else { // stack is full, remove bottom element, and move all other elements and put the new element at the top
        for (int i = 0; i < stack->top; i++) {
            stack->pageNumbers[i] = stack->pageNumbers[i + 1];
        }
        stack->pageNumbers[stack->top] = pageNumber; // put new element at top
    }
}

int findPage(Stack *stack, int page) {
    for (int i = 0; i <= stack->top; i++) {
        if (stack->pageNumbers[i] == page) {
            return i; // return stack index of found page number
        }
    }
    return -1; // page not found
}

// returns 0 if page found (then moved to top)
// returns 1 if not found  (then pushed)
int accessPage(Stack *stack, int pageNumber) {
    int index = findPage(stack, pageNumber);
    if (index != -1) { // page found in stack so just move it to the top
        moveToTop(stack, index);
        return 0;
    }
    // page not found in stack so push it
    pushPage(stack, pageNumber);
    return 1;
}

// # comments for LRU:
// keep in mind: stack contains page numbers. in the page table, the indices are physical frame numbers, and the content at an index is the virtual page number
// implement using stack or linked list, implementation details will vary. opinion: array will be easier since we have to access bottom of the stack and move elements around
// when a page is accessed: two cases
// 1) if the page number doesnt exist in the stack: push it on the top
// 2) if the page number does exist in the stack: move it to the top
// what does this mean? when the page number does exist it means the page table already has an entry for it, it was just accessed again
// if the page number doesnt exist in the stack means it wasnt in the page table as well and will be added in the page table. hence add it on the top of the stack
// at any point, the element at the bottom of the stack will be the least recently used element
// so when the page table is full and an element needs to be replaced, it will be this last element that will be up for replacement



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

    FILE *outfile = fopen("part1out", "w"); // open file part1out for writing
    if (outfile == NULL) {
        printf("Could not open output file\n");
        exit(1);
    }
    int pageFaultCounter = 0;                            // used to count the total pagefaults
    int pageTable[8] = {-1, -1, -1, -1, -1, -1, -1, -1}; // create page table with all values set to -1
    Stack LRUstack;
    initializeStack(&LRUstack);                          // initilize LRU Stack

    // now for each address perform the calculations
    for (int i = 0; i < count; i++) {
        unsigned long address = addresses[i];
        // Extract the page number, first 5 bits
        unsigned long pageNumber = (address >> 7);
        // Extract the offset, last 7 bits
        unsigned long offset = address & 0x7F; // 0x7F is 01111111 in binary

        int frameNumber;
        int frameIndex = findPage(&LRUstack, pageNumber);
        if (frameIndex != -1) { // if frame found
            accessPage(&LRUstack, pageNumber);
        } else {
            pageFaultCounter++;
            // Find free page index
            int freePage = -1;
            for (int j = 0; j < 7; j++) {
                if (pageTable[j] == -1) {
                    freePage = j;
                    frameNumber = j;
                    break;
                }
            }

            // If a frame is available
            if (freePage != -1) {
                // Assign the open page in the pageTable the virtual page number
                pageTable[freePage] = pageNumber;
            } else { // if the pageTable is full
                int LRUPage = LRUstack.pageNumbers[0]; // least recently used page number
                for (int j = 0; j < 7; j++) {
                    if (pageTable[j] == LRUPage) {
                        pageTable[j] = pageNumber;
                        frameNumber = j;
                        pushPage(&LRUstack, pageNumber);
                    }
                }
            }

            // do translation
            unsigned long physical_address = frameNumber * 128 + offset;
            fprintf(outfile, "0x%04lX\n", physical_address);
        }
    }

    fclose(outfile);
    return pageFaultCounter;
}
