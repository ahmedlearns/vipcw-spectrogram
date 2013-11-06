#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         count;  /* count of elements in circular buffer */
    float       *fbuffer;  /* vector of elements                */
} CircularBuffer;

void cbFree(CircularBuffer *cb) {
    free(cb->fbuffer); /* OK if null */ }

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
        ++cb->count;
}

/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, float *elem) {
    *elem = cb->fbuffer[cb->start];
    cb->start = (cb->start + 1) % cb->size;
    --cb->count;
}

void empty_stdin(){
    char c;
    scanf("%c%*[^\n]", &c);
    scanf("%*c");
}

int main(){
    CircularBuffer cb;
    cbInit(&cb, 10);

    while(1){
        float buff[50];
        // char buff[50];
        empty_stdin();
        sleep(1.5);
        int n = read(fileno(stdin), buff, cb.size);
        printf("No. of bytes read: %d\n", n);
        // buff[n] = '\0';
/*
        // char str[100];
        int i = 0;        

        while(i < cb.size){
            // sprintf(str, "%f\t", buff[i]);
            // if(!(i%5)) sprintf(str, "\n");
            printf("%f\t", *(buff+i*sizeof(float)));
            if(!(i%5)) printf("\n");
            ++i;
        }
        printf("\n");
        // printf("%s\n", str);
*/        
        int i = 0;
        while(i < cb.size){
            cbWrite(&cb, &buff[i]);
            i++;
        }

        while(!cbIsEmpty(&cb)){
            float *c;
            printf("cb.count is %d\n", cb.count);
            printf("cb.start is %d\n", cb.start);
            printf("cb.fbuffer[cb.start] is %f\n", cb.fbuffer[cb.start]);
            cbRead(&cb, c);
            printf("here DOS\n");
            printf("%f", *c);
        }

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