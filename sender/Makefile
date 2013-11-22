CC=gcc
CFLAGS=-lm -g
DEP= monofft.c fft_sender.c fft.c

all: $(DEP)
	$(CC) $(DEP) $(CFLAGS) -o sender_child
	$(CC) $(CFLAGS) main.c -o sender

cb: cb_test.c cb_main.c
	$(CC) cb_test.c -o cb_child.out
	$(CC) cb_main.c -o cb.out

nb: nonblock_stdin.c
	$(CC) nonblock_stdin.c -o nb.out
	$(CC) nonblock_stdin2.c -o nb_new.out

select: select_stdin.c
	$(CC) select_stdin.c -o select_stdin.out

sel: sel_example.c nb_read.c
	$(CC) sel_example.c -o sel_example.out
	$(CC) sel_main.c -o sel_main.out
	$(CC) nb_read.c -o nb_read.out

term: terminal_fu.c
	$(CC) terminal_fu.c -o term_fu.out

clena:
clean:
	rm -rf sender sender_child sender.wav *out *exe
