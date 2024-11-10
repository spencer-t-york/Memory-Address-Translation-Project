#ifndef PAGETABLE_H
#define PAGETABLE_H

typedef struct Stack {
    int virtualPageNumbers[7]; // as there are only 7 physical frames to use, it is easier to use 7 here as the size of the stack
    int top;
} Stack;

void initializeStack(Stack *);
void moveToTop(Stack *, int);
void pushPage(Stack *, int);
int findPage(Stack *, int);
int accessPage(Stack *, int);

#endif
