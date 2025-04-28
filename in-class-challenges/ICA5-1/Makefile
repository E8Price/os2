CC = gcc
CFLAGS = -Wall -Wextra

all: main

main: main.o adder.o
	$(CC) -o main main.o adder.o

main.o: main.c adder.h
	$(CC) $(CFLAGS) -c main.c

adder.o: adder.c adder.h
	$(CC) $(CFLAGS) -c adder.c

.PHONY: clean pristine

clean:
	rm -f *.o

pristine: clean
	rm -f main
