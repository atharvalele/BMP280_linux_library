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

/*

example.o: example.c esp.h
	gcc -c -g example.c
esp.o: esp.c esp.h
	gcc -c -g esp.c
example: example.o esp.o
	gcc -o example example.o esp.o -l shunyaInterfaces
*/
