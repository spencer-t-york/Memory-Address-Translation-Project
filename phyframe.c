// first frame is reserved for the OS
// simple page allocation scheme: start from 1,2,3
// if frame is available, then allocate from available
// if not available, use LRU page replacement policy to choose a frame to be replaced
#include <stdlib.h>

typedef struct Node
{
    int pageNumber;
    Node *next;
} Node;

typedef struct Stack
{
    Node *head;
} Stack;

int isEmpty(Stack *stack)
{
    return stack->head == NULL;
}

void push(Stack *stack, int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Stack Overflow!");
        return;
    }
    newNode->pageNumber = data;
    newNode->next = stack->head;
    stack->head = newNode;
}

int pop(Stack *stack)
{
    if (isEmpty(stack))
    {
        printf("Stack Underflow");
        return;
    }

    Node *temp = stack->head;
    stack->head = stack->head->next;

    int pageNumber = temp->pageNumber;
    free(temp);
    return pageNumber;
}
