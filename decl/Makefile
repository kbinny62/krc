CFLAGS+=-std=c99 -pedantic -Wall -I./include

all: decl

decl: decl.o token.o
	$(CC) $(CFLAGS) $+ -o $@ $(LDFLAGS)

clean:
	-rm -f *.o decl
