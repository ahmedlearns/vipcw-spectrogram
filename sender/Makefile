CC=gcc
CFLAGS=-lm
DEP= monofft.c fft_sender.c fft.c

all: $(DEP)
	$(CC) $(DEP) $(CFLAGS) -o sender_child
	$(CC) main.c -o sender

cb: 
	$(CC) cb_test.c -o cb_child
	$(CC) cb_main.c -o ./cb

clean:
	rm -rf sender sender_child cb_child cb
	rm -rf sender.exe sender_child.exe cb_child.exe cb.exe
