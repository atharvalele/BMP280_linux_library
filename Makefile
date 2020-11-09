CC = gcc
CFLAGS = -O2 -g -Wall -Werror
LDFLAGS = -li2c
DEPS = bmp280.h
OBJ = bmp-test.o bmp280.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bmp280: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm *.o bmp280
