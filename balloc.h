#ifndef BALLOC_H
#define BALLOC_H

#include <stddef.h>

void *balloc(size_t size);
void *brealloc(void* ptr, size_t size);
void *bcalloc(size_t num, size_t size);
void bfree(void* memmoryPointer);
void printHeap();

#endif