#ifndef SELLER_H
#define SELLER_H

void init_seller_queues(int customers_per_seller);
void *seller_thread(void *arg);

#endif