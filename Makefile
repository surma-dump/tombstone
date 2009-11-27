CFLAGS=-I. -I/usr/include 
all:
	gcc tombstone.c -o tombstone ${CFLAGS}
	gcc tombstone.c -o tombstone_debug -ggdb ${CFLAGS}
gdb:
	gdb ./tombstone_debug
run:
	./tombstone
clean:
	@rm -f tombstone tombstone_debug *.o
