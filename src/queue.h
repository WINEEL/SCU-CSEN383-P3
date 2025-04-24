#ifndef QUEUE_H
#define QUEUE_H

typedef struct customer {
    int id;
    int arrival_time;
} customer_t;

typedef struct node {
    customer_t *data;
    struct node *next;
} node_t;

typedef struct queue {
    node_t *head;
    node_t *tail;
    int size;
} queue_t;

queue_t *create_queue();
void enqueue(queue_t *q, customer_t *cust);
customer_t *dequeue(queue_t *q);
void sort_queue_by_arrival(queue_t *q);

#endif
