#include <stdlib.h>
#include "queue.h"

queue_t *create_queue() {
    queue_t *q = malloc(sizeof(queue_t));
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

void enqueue(queue_t *q, customer_t *cust) {
    node_t *node = malloc(sizeof(node_t));
    node->data = cust;
    node->next = NULL;

    if (!q->tail) {
        q->head = q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }

    q->size++;
}

customer_t *dequeue(queue_t *q) {
    if (!q->head) return NULL;
    node_t *temp = q->head;
    customer_t *data = temp->data;
    q->head = q->head->next;
    if (!q->head) q->tail = NULL;
    free(temp);
    q->size--;
    return data;
}

void sort_queue_by_arrival(queue_t *q) {
    if (!q || q->size <= 1) return;
    for (node_t *i = q->head; i && i->next; i = i->next) {
        for (node_t *j = i->next; j; j = j->next) {
            if (i->data->arrival_time > j->data->arrival_time) {
                customer_t *tmp = i->data;
                i->data = j->data;
                j->data = tmp;
            }
        }
    }
}
