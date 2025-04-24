#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "seller.h"
#include "queue.h"
#include "seatmap.h"

extern int current_minute;
extern int simulation_ended;
extern int active_thread;

pthread_mutex_t clock_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clock_cond = PTHREAD_COND_INITIALIZER;

queue_t *seller_queues[10];

void init_seller_queues(int customers_per_seller) {
    for (int i = 0; i < 10; i++) {
        seller_queues[i] = create_queue();
        for (int j = 0; j < customers_per_seller; j++) {
            customer_t *cust = malloc(sizeof(customer_t));
            cust->id = j + 1;
            cust->arrival_time = rand() % 60;
            enqueue(seller_queues[i], cust);
        }
        sort_queue_by_arrival(seller_queues[i]);
    }
}

void *seller_thread(void *arg) {
    int seller_id = *((int *)arg);
    free(arg);

    char seller_type = (seller_id == 0) ? 'H' : (seller_id <= 3) ? 'M' : 'L';
    queue_t *q = seller_queues[seller_id];

    customer_t *serving = NULL;
    int service_time = 0;

    // Track thread activity
    pthread_mutex_lock(&clock_mutex);
    active_thread++;
    pthread_mutex_unlock(&clock_mutex);

    while (1) {
        pthread_mutex_lock(&clock_mutex);
        pthread_cond_wait(&clock_cond, &clock_mutex);
        int time_now = current_minute;
        int done = simulation_ended;
        pthread_mutex_unlock(&clock_mutex);

        if (done || time_now >= 60) break;

        while (q->size > 0 && q->head->data->arrival_time <= time_now) {
            if (!serving) {
                serving = dequeue(q);
                if (serving) {
                    switch (seller_type) {
                        case 'H': service_time = rand() % 2 + 1; break;
                        case 'M': service_time = rand() % 3 + 2; break;
                        case 'L': service_time = rand() % 4 + 4; break;
                    }
                    printf("%02d:00 Seller %c%d starts serving C%02d\n", time_now, seller_type, seller_id, serving->id);
                }
            } else {
                break;
            }
        }

        if (serving) {
            service_time--;
            if (service_time == 0) {
                if (assign_seat(seller_type, seller_id, serving->id, time_now)) {
                    printf("%02d:00 Seller %c%d completed serving C%02d\n", time_now, seller_type, seller_id, serving->id);
                } else {
                    printf("%02d:00 Seller %c%d Customer No C%02d left - Sold Out\n", time_now, seller_type, seller_id, serving->id);
                }
                free(serving);
                serving = NULL;
            }
        }
    }

    // Final cleanup for any customers still in queue
    while (serving || q->size > 0) {
        if (!serving) {
            serving = dequeue(q);
        }
        if (serving) {
            printf("%02d:00 Seller %c%d Ticket Sale Closed. Customer No C%02d Leaves\n",
                   current_minute, seller_type, seller_id, serving->id);
            free(serving);
            serving = NULL;
        }
    }

    pthread_mutex_lock(&clock_mutex);
    active_thread--;
    pthread_mutex_unlock(&clock_mutex);

    return NULL;
}
