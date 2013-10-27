CC=gcc
GTKFLAGS=`pkg-config --libs --cflags gtk+-2.0`

all: gtk_final.c
	$(CC) testing.c $(GTKFLAGS) -o testing

clean:
	rm -rf receiver
