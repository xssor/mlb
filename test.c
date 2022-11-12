#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

#include "mlb.h";

int main() {
    // initialize the gabage collector
    gc_init();

    // create some random data blocks
    gc_malloc(1024);
    gc_malloc(12);
    gc_malloc(66);
    gc_malloc(66);

    if(true) {
        volatile char *str = gc_malloc(64);
        gc_collect();
    }
    return 0;
}