CC=gcc
CFLAGS=-I.
DEPS = history.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

masterminds: main.c history.o
	gcc -w -o masterminds_v2.out main.c history.o
	./masterminds_v2.out
