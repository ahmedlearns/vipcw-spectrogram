CC=gcc

all: rec.c linkedlist.o
		$(CC) rec.c linkedlist.o $(GTKFLAGS) -o rec
linkedlist.o: linkedlist.h 
        
clean:
		rm -rf rec
		rm -rf linkedlist.o