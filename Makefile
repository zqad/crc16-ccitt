CFLAGS+=-std=c99 -pedantic -Wall -Wshadow -Wpointer-arith -Wstrict-prototypes

CFLAGS+=-g

.PHONY: clean

example: example.o example-table.o example-algorithm.o crc16-ccitt-table.o 

crc16-ccitt-generate-table: crc16-ccitt-generate-table.o

crc16-ccitt-table.c: crc16-ccitt-generate-table
	./$^ > $@

example-table.o: example-table.c crc16-ccitt-table.h

example-algorithm.o: example-algorithm.c crc16-ccitt-algorithm.h

clean:
	rm -f example crc16-ccitt-generate-table *.o crc16-ccitt-table.c
