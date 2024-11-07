// first frame is reserved for the OS
// simple page allocation scheme: start from 1,2,3
// if frame is available, then allocate from available
// if not available, use LRU page replacement policy to choose a frame to be replaced
#include <stdlib.h>

typedef struct Stack
{
    int pageNumbers[7]; // as there are only 7 physical frames to use, it is easier to use 7 here as the size of the stack
    int top;
} Stack;

void initializeStack(Stack *stack)
{
    stack->top = -1;
}

void moveToTop(Stack *stack, int index)
{
    int pageNumber = stack->pageNumbers[index];
    for (int i = index; i < stack->top; i++)
    {
        stack->pageNumbers[i] = stack->pageNumbers[i + 1];
    }
    stack->pageNumbers[stack->top] = pageNumber; // move accessed page number to the top
}

void pushPage(Stack *stack, int pageNumber)
{
    if (stack->top != 6) // stack is not full and we can just put the page number at the top
    {
        stack->pageNumbers[++stack->top] = pageNumber;
    }
    else // stack is full, remove bottom element, and move all other elements and put the new element at the top
    {
        for (int i = 0; i < stack->top; i++)
        {
            stack->pageNumbers[i] = stack->pageNumbers[i + 1];
        }
        stack->top--; // we just removed an element from the stack so decrement top
        stack->pageNumbers[++stack->top] = pageNumber; // put new element at top
    }
}

int findPage(Stack *stack, int page)
{
    for (int i = 0; i < stack->top; i++)
    {
        if (stack->pageNumbers[i] == page)
        {
            return i; // return stack index of found page number
        }
    }
    return -1; // page not found
}

void accessPage(Stack *stack, int pageNumber)
{
    int index = findPage(stack, pageNumber);
    if (index != -1)
    { // page found in stack so just move it to the top
        moveToTop(stack, pageNumber);
    }
    else
    { // page not found in stack so push it
        pushPage(stack, pageNumber);
    }
}
