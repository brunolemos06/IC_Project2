#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    size_t index;
    uint32_t * items;
    int is_full;
    int size;
} circular_buffer;

circular_buffer *cb_init(int size){
    //allocate memory for items
    circular_buffer *cb = (circular_buffer *)malloc(sizeof(circular_buffer));
    cb->items = new uint32_t[size];
    cb->index = 0;
    cb->is_full = 0;
    cb->size = size;
    return cb;
}

void cb_push(circular_buffer *cb, uint32_t item){
    cb->items[cb->index] = item;
    cb->index = (cb->index + 1) % cb->size;
    cb->is_full = cb->is_full || (cb->index == 0);
}

int cb_is_full(circular_buffer *cb){
    return cb->is_full;
}

int cb_get_index(circular_buffer *cb, int index){
    if (index >= cb->size){
        printf("ERROR: index out of bounds");
        return -1;
    }
    return cb->items[index];
}
    

void cb_average(circular_buffer *cb, uint32_t *average){
    uint32_t sum = 0;
    for (int i = 0; i < cb->size; i++){
        //printf("HAVE %d ADDING %d ", sum, cb->items[i]);
        sum += cb->items[i];
        //printf("GOT %d\n",sum);
    }
    //average = 1-sum/sum;
    *average = (uint32_t) (sum / cb->size);
}

void cb_clear(circular_buffer *cb){
    cb->index = 0;
    cb->is_full = 0;
    cb->items = new uint32_t[cb->size];
}

void cb_print(circular_buffer *cb){
    printf("ARRAY: [");
    for (int i = 0; i < cb->size; i++){
        printf("%d, ", cb->items[i]);
    }
    printf("]\n");
}

#endif
