bmp-test.o: bmp-test.c bmp280.h
	gcc -c -g bmp-test.c
bmp280.o: bmp280.c bmp280.h
	gcc -c -g bmp280.c
bmp-test: bmp-test.o bmp280.o
	gcc -o bmp-test bmp-test.o bmp280.o 