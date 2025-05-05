CC = gcc
CFLAGS = -Wall -Wextra -Werror

OBJS = image.o block.o
LIB = libvvsfs.a
EXEC = testfs

.PHONY: all test clean pristine

all: $(LIB) $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	ar rcs $@ $^

$(EXEC): testfs.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

test: $(EXEC)
	./$(EXEC)

clean:
	rm -f *.o

pristine: clean
	rm -f $(EXEC) $(LIB) test.img

