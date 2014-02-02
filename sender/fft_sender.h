/* fft_sender.h
 *
 * This file will contain headers for functions that will package fft data 
 * to be sent through a socket.
 * 
 * Author: Ahmed Ismail
 */

/* Default Values */
#define DEFAULT_D "default"; 
#define DEFAULT_P 51717;
#define DEFAULT_R 22050;
#define DEFAULT_S 256;
#define DEFAULT_F 10;
#define DEFAULT_T 0.75;
#define DEFAULT_W 0.99; 

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
