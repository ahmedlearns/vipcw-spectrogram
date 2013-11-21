/* fft_sender.h
 *
 * This file will contain headers for functions that will package fft data 
 * to be sent through a socket.
 * 
 * Author: Ahmed Ismail
 */

 /* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         count;  /* count of elements in circular buffer */
    float       *fbuffer;  /* vector of elements                */
} CircularBuffer;

void cbFree(CircularBuffer *cb);

void cbInit(CircularBuffer *cb, int size);

int cbIsFull(CircularBuffer *cb);
 
int cbIsEmpty(CircularBuffer *cb);

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, float *elem);

/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, float *elem);

// void init_fft(int, int, int, struct timeval, int);
void init_fft(int, int, int, int, int, int);

//void updateTime(struct timeval);

void err(const char*);

int discard_wav_header();
