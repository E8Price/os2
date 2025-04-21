CFLAGS = -Wall -Wextra -Werror
OBJ = pc.o eventbuf.o

pc: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f pc *.o
