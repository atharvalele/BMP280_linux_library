CC = gcc
CFLAGS = -O2 -g -Wall -Werror
LDFLAGS = -li2c
DEPS = bmp280.h bmp280.c
OBJ = bmp-test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bmp280: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm *.o