CC=gcc
CFLAGS=-lm
DEP= monofft.c fft_sender.c fft.c

all: $(DEP)
	$(CC) $(DEP) $(CFLAGS) -o sender_child
	$(CC) main.c -o sender

cb: 
	$(CC) cb_test.c -o cb_test

clean:
	rm -rf sender sender_child cb_test
	rm -rf sender.exe sender_child.exe cb_test.exe
