#ifndef SEATMAP_H
#define SEATMAP_H

void init_seatmap();
int assign_seat(char seller_type, int seller_no, int cust_no, int minute);
void print_seatmap();
void print_statistics(int customers_per_seller);

#endif
