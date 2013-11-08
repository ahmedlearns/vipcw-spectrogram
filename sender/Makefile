CC=gcc
CFLAGS=-lm
DEP= monofft.c fft_sender.c fft.c

all: $(DEP)
	$(CC) $(DEP) $(CFLAGS) -o sender_child
	$(CC) main.c -o sender

cb: cb_test.c cb_main.c
	$(CC) cb_test.c -o cb_child.out
	$(CC) cb_main.c -o cb.out

nb: nonblock_stdin.c
	$(CC) nonblock_stdin.c -o nb.out
	$(CC) nonblock_stdin2.c -o nb_new.out

select: select_stdin.c
	$(CC) select_stdin.c -o select_stdin.out

clena:
clean:
	rm -rf sender sender_child cb_child cb sender.wav nb nb_new *out
	rm -rf sender.exe sender_child.exe cb_child.exe cb.exe
