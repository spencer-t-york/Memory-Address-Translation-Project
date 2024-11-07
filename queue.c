#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "sthread.h"
#include "sync.h"

// Create writer queue
Queue *createWriterQueue() {
    Queue *w_queue = (Queue *)malloc(sizeof(Queue)); // create writer queue
    w_queue->front = NULL;
    w_queue->rear = NULL;
    return w_queue;    // return pointer to r_queue
}

// Add thread to back of queue
void enqueue(Queue *queue, sthread_t thread) {
    Node *newNode = (Node *)malloc(sizeof(Node)); // Allocate memory for new node
    newNode->thread = thread;                     // Have the new node contain the thread passed in to enqueue()

    newNode->next = NULL;                         // set the next node after new node to NULL (end of list)
    if (queue->rear == NULL) {                    // if queue is empty
        queue->front = newNode;                   //     set the newNode as the front element
        queue->rear = newNode;                    //     set the newNode as the rear element
        newNode->prev = NULL;                     //     set the newNode's prev to the NULL
    } else {                                      // else if queue is not empty
        queue->rear->next = newNode;              //     set the newNode as the rear's next
        newNode->prev = queue->rear;              //     set the newNode's prev to the current rear
        queue->rear = newNode;                    //     move the pointer rear to the newNode
    }

}
// Remove element from front of queue
sthread_t dequeue(Queue *queue) {
    if (queue->front == NULL) { // if queue is empty...
        return NULL;
    }
    Node *front = queue->front;
    sthread_t thread = queue->front->thread;     // store dequeued thread
    queue->front = queue->front->next;  // set front to the next element

    if (queue->front == NULL) {         // if queue is empty
        queue->rear = NULL;             //     set rear to NULL
    } else {                            // if queue is not empty
        queue->front->prev = NULL;      //     set the previous queue to NULL
    }
    free(front);
    return thread;
}

// Determine if queue is empty
int isEmpty(Queue *queue) {
    return queue->front == NULL;
}
