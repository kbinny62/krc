
INCDIR=include/
CFLAGS+=-I$(INCDIR) -g
LDFLAGS+=-fsanitize=address

sort: sort.o argv.o qsort.o

argv.o: argv.c include/argv.h
	$(CC) $(CFLAGS) -std=c99 -D_POSIX_C_SOURCE=200809L -c -o $@ argv.c

clean:
	-rm -f *.o sort

