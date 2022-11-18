#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include "mlb.h"

#define ARRAY_SIZE 32

void use_memory();

int main() {
    // initialize the gabage collector
    gc_init();

    // create some random data blocks
    // gc_malloc(1024);
    // gc_malloc(12);
    // gc_malloc(66);
    // gc_malloc(66);

    use_memory();

    if(true) {
        volatile char *str = gc_malloc(64);
        gc_collect();
    }
    int *foo = gc_malloc(sizeof(int));
    *foo = 5;
    int *bar = gc_malloc(sizeof(int));
    *bar = 69420;
    printf("foo = %d\n", *foo);
    printf("bar = %d\n", *bar);
    gc_init();
    return 0;
}

void use_memory() {
    long *three = gc_malloc(sizeof(long));
    *three = 51;
    long *four = gc_malloc(sizeof(long));
    *four = 99;

    int *arr = gc_malloc(sizeof(int) * ARRAY_SIZE);
    for(int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = ARRAY_SIZE - (i * 2);
    }
    for(int i = 0; i < ARRAY_SIZE; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    printf("\n");
}


