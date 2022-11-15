#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SIZE 3

typedef struct {
    size_t index;
    int items[MAX_SIZE];
    int is_full;
} circular_buffer;

circular_buffer *cb_init(){
    circular_buffer *cb = (circular_buffer *)malloc(sizeof(circular_buffer));
    cb->index = 0;
    cb->is_full = 0;
    return cb;
}

void cb_push(circular_buffer *cb, int item){
    cb->items[cb->index] = item;
    cb->index = (cb->index + 1) % MAX_SIZE;
    cb->is_full = cb->is_full || (cb->index == 0);
}

int cb_is_full(circular_buffer *cb){
    return cb->is_full;
}

void cb_average(circular_buffer *cb, double *average){
    double sum = 0;
    for (int i = 0; i < MAX_SIZE; i++){
        //printf("HAVE %d ADDING %d ", sum, cb->items[i]);
        sum += cb->items[i];
        //printf("GOT %d\n",sum);
    }
    //average = 1-sum/sum;
    *average = (double) (sum / MAX_SIZE);
}

void cb_print(circular_buffer *cb){
    printf("ARRAY: [");
    for (int i = 0; i < MAX_SIZE; i++){
        printf("%d, ", cb->items[i]);
    }
    printf("]\n");
}

#endif
