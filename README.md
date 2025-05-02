# Concert Ticket Sellers Simulation

This project simulates a multi-threaded environment where 10 ticket sellers distribute 100 seats in a 10x10 concert hall over a 60-minute simulation window. The implementation uses **Pthreads in C**, including synchronization primitives like **mutexes** and **condition variables** to manage concurrency.

## Seller Configuration

- **1 High-Priced Seller (H)**
  - Allocates seats from front (row 0) to back
- **3 Medium-Priced Sellers (M)**
  - Start from the middle row (row 5) and assign alternately outward
- **6 Low-Priced Sellers (L)**
  - Assign from back (row 9) to front

Each seller has a queue of `N` customers (where N is passed as a command-line argument). Customers arrive at random minutes between 0 and 59. Each seller serves customers based on service time rules:

- **H**: 1–2 minutes per customer
- **M**: 2–4 minutes per customer
- **L**: 4–7 minutes per customer

## Simulation Time

Simulation runs from minute 0 to 59 (total 60 minutes). The clock is simulated in real time using either:

- `sleep(1)` (1 second = 1 minute) for visual clarity, or
- `usleep(100000)` (0.1 second = 1 minute) for faster testing.

### Current Setting
The simulation is currently running in **FAST MODE** (`usleep(100000)`) to speed up testing. This can be toggled in `main.c`:

```c
#define FAST_MODE 1  // Set to 0 for normal mode
```

## Project Structure

```plaintext
.
├── src/
│   ├── main.c              # Main simulation loop and clock logic
│   ├── seller.c/.h         # Thread logic for each seller
│   ├── queue.c/.h          # Customer queue implementation
│   ├── seatmap.c/.h        # Seat assignment and final statistics
│   └── Makefile            # Makefile for compilation using 'make'
├── output/
│   ├── Output1.txt         # N=5 simulation
│   ├── Output2.txt         # N=10 simulation
│   └── Output3.txt         # N=15 simulation
├── README.md               # This file
└── Project 3 Report.txt    # Project documentation
```

## Compilation Instructions

### Using Make (Recommended)
From the `src/` directory, run:

```bash
make
```

This compiles the project into an executable named `main`.

To remove compiled files:
```bash
make clean
```

### Makefile Example
The `src/Makefile` looks like this:

```makefile
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
```

### Without Make

Use the following command to compile:

```bash
gcc -o main main.c queue.c seller.c seatmap.c -lpthread
```

If you prefer using `make`, you can create a Makefile with appropriate rules.

## Running the Simulation

```bash
./main <customers_per_seller>

# Example
./main 5
```

To save output:
```bash
./main 5 > output/Output1.txt
```

## Output Includes
- Per-minute logs of customer arrivals, service, and seat assignments
- Final concert hall seating chart (10x10)
- Summary statistics:
  - Total customers served by H, M, L sellers
  - Customers turned away
  - Placeholder average Turnaround Time, Response Time, Throughput

## Notes
- Ensure the `output/` folder exists before redirecting output.
- Fast mode helps reduce waiting time during tests.
- Customer seat assignment follows the type-based row rules strictly.
- Mutexes ensure only one seller assigns a seat at a time.
