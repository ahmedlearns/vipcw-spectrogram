#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         count;  /* count of elements in circular buffer */
    char       *fbuffer;  /* vector of elements                */
} CircularBuffer;

void cbFree(CircularBuffer *cb) {
    free(cb->fbuffer); /* OK if null */ }

void cbInit(CircularBuffer *cb, int size) {
    cb->size  = size;
    cb->start = 0;
    cb->count = 0;
    cb->fbuffer = (char*) calloc(cb->size, sizeof(char));
}
 
int cbIsFull(CircularBuffer *cb) {
    return cb->count == cb->size; }
 
int cbIsEmpty(CircularBuffer *cb) {
    return cb->count == 0; }

/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, char *elem) {
    int end = (cb->start + cb->count) % cb->size;
    cb->fbuffer[end] = *elem;
    if (cb->count == cb->size)
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
    else
        ++ cb->count;
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, char *elem) {
    *elem = cb->fbuffer[cb->start];
    cb->start = (cb->start + 1) % cb->size;
    -- cb->count;
}

void empty_stdin(){
    char c;
    scanf("%c%*[^\n]", &c);
    scanf("%*c");
}

int main(){
    CircularBuffer cb;
    cbInit(&cb, 10);
    // float a = 5, b = 10, c = 15, d = 20;
    // cbWrite(&cb, &a);
    // cbWrite(&cb, &b);
    // cbWrite(&cb, &c);
    // cbWrite(&cb, &d);

    while(1){
        float buff[50];
        empty_stdin();
        sleep(1.5);
        int n = read(fileno(stdin), buff, cb.size);
        int i = 0;
        for(; i < 50; i++)
            printf("%f\n", buff[i]);
    }

    // char buff[50];
    // int n;
    // while( (n = read(fileno(stdin), buff, cb.size)) > 0){
    //     buff[n] = '\0';
    //     int i = 0;
    //     while(buff[i] != 0){
    //         cbWrite(&cb, &buff[i]);
    //         i++;
    //     }
    //     while(!cbIsEmpty(&cb)){
    //         char *c;
    //         cbRead(&cb, c);
    //         printf("%c", *c);
    //     }
    // }
}