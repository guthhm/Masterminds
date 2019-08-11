CC=gcc
CFLAGS=-I.
DEPS = oraculo.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

masterminds: main.c oraculo.o
	gcc -w -o masterminds_v2.out main.c
	./masterminds_v2.out
