
INCDIRS=-I./include/ -I../external/argv/
CFLAGS+=$(INCDIRS) -g
#LDFLAGS+=-fsanitize=address

sort: sort.o ../external/argv/argv.o qsort.o
	$(LD) $(LDFLAGS) $+ -o $@

.PHONY: ../external/argv/argv.o
../external/argv/argv.o:
	#$(CC) $(CFLAGS) -std=c99 -D_POSIX_C_SOURCE=200809L -c -o $@ argv.c
	make -C ../external/argv

clean:
	-rm -f *.o sort

