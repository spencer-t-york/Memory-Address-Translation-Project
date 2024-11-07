// Node Structure
typedef struct Node {
    sthread_t thread;
    struct Node *prev;
    struct Node *next;
} Node;

 // Queue Structure
typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;
