CC=gcc
CFLAGS=-lm
DEP= monofft.c fft_sender.c fft.c

all: $(DEP)
	$(CC) $(DEP) $(CFLAGS) -o sender 

clean:
	rm -rf sender 
