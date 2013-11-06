CC=gcc
GTKFLAGS=`pkg-config --libs --cflags gtk+-2.0`

all: receiver.c
	$(CC) receiver.c $(GTKFLAGS) -o receiver

clean:
	rm -rf receiver
