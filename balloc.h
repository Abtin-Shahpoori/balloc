#ifndef BALLOC_H
#define BALLOC_H

#include <stddef.h>

void *balloc(size_t size);
void bfree(void* memmoryPointer);
void printHeap();

#endif