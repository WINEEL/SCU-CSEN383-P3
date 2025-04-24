#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.h"
#include "seller.h"
#include "seatmap.h"

#define SELLER_COUNT 10
#define SIMULATION_DURATION 60

pthread_t seller_threads[SELLER_COUNT];
extern pthread_mutex_t clock_mutex;
extern pthread_cond_t clock_cond;

int current_minute = 0;
int simulation_ended = 0;
int active_thread = 0;
int customers_per_seller;

// SIMULATION SPEED TOGGLE
// ------------------------
// Set FAST_MODE to:
//    1 → Fast mode (0.1 second = 1 simulated minute)
//    0 → Normal mode (1 second = 1 simulated minute)
#define FAST_MODE 1

void *seller_thread(void *arg);
void wakeup_all_seller_threads();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <customers_per_seller>\n", argv[0]);
        return 1;
    }

    customers_per_seller = atoi(argv[1]);
    init_seatmap();
    init_seller_queues(customers_per_seller);

    for (int i = 0; i < SELLER_COUNT; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&seller_threads[i], NULL, seller_thread, id);
    }

    for (current_minute = 0; current_minute < SIMULATION_DURATION; current_minute++) {
        if (FAST_MODE) {
            usleep(100000); // Fast mode (0.1s per minute)
        } else {
            sleep(1);       // Normal mode (1s per minute)
        }
        pthread_mutex_lock(&clock_mutex);
        pthread_cond_broadcast(&clock_cond);
        pthread_mutex_unlock(&clock_mutex);
    }

    simulation_ended = 1;
    pthread_mutex_lock(&clock_mutex);
    pthread_cond_broadcast(&clock_cond);
    pthread_mutex_unlock(&clock_mutex);

    while (active_thread > 0) {
        sleep(1);
    }

    print_seatmap();
    print_statistics(customers_per_seller);

    return 0;
}
