# Makefile for Project 3 - Ticket Seller Simulation

CC = gcc
CFLAGS = -Wall -lpthread
SRC = main.c seller.c queue.c seatmap.c
EXEC = main

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

clean:
ifeq ($(OS),Windows_NT)
	del /Q $(EXEC).exe 2>nul || exit 0
else
	rm -f $(EXEC) *.o
endif
