CC = gcc
CFLAGS = -O2 -g -Wall -Werror
DEPS = bmp280.h
OBJ = bmp-test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bmp280: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o
