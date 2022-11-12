#pragma once

#include <stdlib.h>
#include <stdint.h>

void gc_collect();
void * gc_malloc(size_t alloc_size);
void gc_init();




