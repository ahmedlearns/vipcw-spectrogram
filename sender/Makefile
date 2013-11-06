CC=gcc
CFLAGS=-lm
DEP= monofft.c fft_sender.c fft.c

all: $(DEP)
	$(CC) $(DEP) $(CFLAGS) -o sender_child
	$(CC) main.c -o sender

cb: cb_test.c cb_main.c
	$(CC) cb_test.c -o cb_child
	$(CC) cb_main.c -o cb

nb: nonblock_stdin.c
	$(CC) nonblock_stdin.c -o nb

clena:
clean:
	rm -rf sender sender_child cb_child cb sender.wav
	rm -rf sender.exe sender_child.exe cb_child.exe cb.exe
