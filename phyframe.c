// NOTE: this stack will have to be modified and additional functions will have to be added according to the comments at the bottom of mempart2.c
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
