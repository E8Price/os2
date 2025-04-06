CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = hellothread

all: $(TARGET)

$(TARGET): hellothread.c
	$(CC) $(CFLAGS) -o $(TARGET) hellothread.c -pthread

clean:
	rm -f $(TARGET)
