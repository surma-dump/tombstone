CFLAGS=-I. -I/usr/include `pkg-config --cflags glib-2.0` -DDEBUG
LDFLAGS=`pkg-config --libs glib-2.0`
all:
	gcc tombstone.c -o tombstone ${CFLAGS} ${LDFLAGS}
	gcc tombstone.c -o tombstone_debug -ggdb ${CFLAGS} ${LDFLAGS}
gdb:
	gdb ./tombstone_debug
run:
	./tombstone
clean:
	@rm -f tombstone tombstone_debug *.o
