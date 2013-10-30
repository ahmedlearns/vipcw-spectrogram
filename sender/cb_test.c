#include <stdio.h>
#include <stdlib.h>

/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         count;  /* count of elements in circular buffer */
    float       *fbuffer;  /* vector of elements                */
} CircularBuffer;

void cbFree(CircularBuffer *cb) {
    free(cb->elems); /* OK if null */ }

void cbInit(CircularBuffer *cb, int size) {
    cb->size  = size;
    cb->start = 0;
    cb->count = 0;
    cb->fbuffer = (float*) calloc(cb->size, sizeof(float));
}
 
int cbIsFull(CircularBuffer *cb) {
    return cb->count == cb->size; }
 
int cbIsEmpty(CircularBuffer *cb) {
    return cb->count == 0; }

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, float *elem) {
    int end = (cb->start + cb->count) % cb->size;
    cb->fbuffer[end] = *elem;
    if (cb->count == cb->size)
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
    else
        ++ cb->count;
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, float *elem) {
    *elem = cb->fbuffer[cb->start];
    cb->start = (cb->start + 1) % cb->size;
    -- cb->count;
}

int main(){
    
}