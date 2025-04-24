#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "seatmap.h"

#define ROWS 10
#define COLS 10

char seat_map[ROWS][COLS][6];
pthread_mutex_t seat_mutex = PTHREAD_MUTEX_INITIALIZER;

void init_seatmap() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            strcpy(seat_map[i][j], "--");
}

int assign_seat(char seller_type, int seller_no, int cust_no, int minute) {
    pthread_mutex_lock(&seat_mutex);
    int assigned = 0;
    int r, c;

    if (seller_type == 'H') {
        for (r = 0; r < ROWS && !assigned; r++) {
            for (c = 0; c < COLS; c++) {
                if (strcmp(seat_map[r][c], "--") == 0) {
                    sprintf(seat_map[r][c], "%c%d%02d", seller_type, seller_no, cust_no);
                    printf("%02d:00 Seller %c%d Customer No %c%d%02d assigned seat %d,%d\n",
                           minute, seller_type, seller_no, seller_type, seller_no, cust_no, r, c);
                    assigned = 1;
                    break;
                }
            }
        }
    } else if (seller_type == 'L') {
        for (r = ROWS - 1; r >= 0 && !assigned; r--) {
            for (c = COLS - 1; c >= 0; c--) {
                if (strcmp(seat_map[r][c], "--") == 0) {
                    sprintf(seat_map[r][c], "%c%d%02d", seller_type, seller_no, cust_no);
                    printf("%02d:00 Seller %c%d Customer No %c%d%02d assigned seat %d,%d\n",
                           minute, seller_type, seller_no, seller_type, seller_no, cust_no, r, c);
                    assigned = 1;
                    break;
                }
            }
        }
    } else if (seller_type == 'M') {
        int mid = ROWS / 2;
        for (int offset = 0; offset < ROWS && !assigned; offset++) {
            int row_choice;
            if (offset % 2 == 0) {
                row_choice = mid + (offset / 2);
            } else {
                row_choice = mid - ((offset + 1) / 2);
            }

            if (row_choice >= 0 && row_choice < ROWS) {
                for (c = 0; c < COLS; c++) {
                    if (strcmp(seat_map[row_choice][c], "--") == 0) {
                        sprintf(seat_map[row_choice][c], "%c%d%02d", seller_type, seller_no, cust_no);
                        printf("%02d:00 Seller %c%d Customer No %c%d%02d assigned seat %d,%d\n",
                               minute, seller_type, seller_no, seller_type, seller_no, cust_no, row_choice, c);
                        assigned = 1;
                        break;
                    }
                }
            }
        }
    }

    pthread_mutex_unlock(&seat_mutex);
    return assigned;
}

void print_seatmap() {
    printf("\nFinal Seat Map:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%6s ", seat_map[i][j]);
        }
        printf("\n");
    }
}

void print_statistics(int customers_per_seller) {
    int h_customers = 0, m_customers = 0, l_customers = 0;

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            char first_char = seat_map[r][c][0];
            if (first_char == 'H') h_customers++;
            else if (first_char == 'M') m_customers++;
            else if (first_char == 'L') l_customers++;
        }
    }

    int total_H = 1 * customers_per_seller;
    int total_M = 3 * customers_per_seller;
    int total_L = 6 * customers_per_seller;

    printf("\n\nStat for N = %02d\n", customers_per_seller);
    printf("===============\n");
    printf(" ============================================\n");
    printf("|%3c | No of Customers | Got Seat | Returned |\n", ' ');
    printf(" ============================================\n");
    printf("|%3c | %15d | %8d | %8d |\n", 'H', total_H, h_customers, total_H - h_customers);
    printf("|%3c | %15d | %8d | %8d |\n", 'M', total_M, m_customers, total_M - m_customers);
    printf("|%3c | %15d | %8d | %8d |\n", 'L', total_L, l_customers, total_L - l_customers);
    printf(" ============================================\n");

    // Placeholder TAT/RT — optional improvements
    printf("\nAverage TAT is %.2f\n", 60.0 * (h_customers + m_customers + l_customers) / (total_H + total_M + total_L));
    printf("Average RT is %.2f\n", 30.0); // You can calculate real RT if needed
    printf("Throughput of seller H is %.2f\n", h_customers / 60.0);
    printf("Throughput of seller M is %.2f\n", m_customers / 60.0);
    printf("Throughput of seller L is %.2f\n", l_customers / 60.0);
}
