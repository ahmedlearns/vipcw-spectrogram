CC=gcc
GTKFLAGS=`pkg-config --libs --cflags gtk+-2.0`

all: gtk_final.c
	$(CC) gtk_final.c $(GTKFLAGS) -o gtk_final

clean:
	rm -rf testing

