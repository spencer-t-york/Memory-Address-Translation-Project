#include "pagetable.h"
// first frame is reserved for the OS
// simple page allocation scheme: start from 1,2,3
// if frame is available, then allocate from available
// if not available, use LRU page replacement policy to choose a frame to be replaced

void initializeStack(Stack *stack) {
    stack->top = -1;
}

void moveToTop(Stack *stack, int index) {
    int virtualPageNumber = stack->virtualPageNumbers[index];
    for (int i = index; i < stack->top; i++) {
        stack->virtualPageNumbers[i] = stack->virtualPageNumbers[i + 1];
    }
    stack->virtualPageNumbers[stack->top] = virtualPageNumber; // move accessed page number to the top
}

void pushPage(Stack *stack, int virtualPageNumber) {
    if (stack->top < 6) { // stack is not full and we can just put the page number at the top
        stack->virtualPageNumbers[++stack->top] = virtualPageNumber;
    }
    else { // stack is full, remove bottom element, and move all other elements and put the new element at the top
        for (int i = 0; i < stack->top; i++) {
            stack->virtualPageNumbers[i] = stack->virtualPageNumbers[i + 1];
        }
        stack->virtualPageNumbers[stack->top] = virtualPageNumber; // put new element at top
    }
}

int findPage(Stack *stack, int page) {
    for (int i = 0; i <= stack->top; i++) {
        if (stack->virtualPageNumbers[i] == page) {
            return i; // return stack index of found page number
        }
    }
    return -1; // page not found
}

// returns 0 if page found (then moved to top)
// returns 1 if not found  (then pushed)
int accessPage(Stack *stack, int virtualPageNumber) {
    int index = findPage(stack, virtualPageNumber);
    if (index != -1) { // page found in stack so just move it to the top
        moveToTop(stack, index);
        return 0;
    }
    // page not found in stack so push it
    pushPage(stack, virtualPageNumber);
    return 1;
}