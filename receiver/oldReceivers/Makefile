CC=gcc
GTKFLAGS=`pkg-config --libs --cflags gtk+-2.0`

all: testing.c
	$(CC) testing.c $(GTKFLAGS) -o testing

clean:
	rm -rf testing

