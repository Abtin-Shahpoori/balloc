#include "balloc.h"
#include <unistd.h>
#include <stdio.h>

#define ALIGNMENT 16
#define Align(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define HEADER_SIZE (sizeof(struct balloc_Chunk))

struct balloc_Chunk {
    int isAllocated;
    size_t size;

    struct balloc_Chunk *next;
    struct balloc_Chunk *previous;
};


int breakChunk(struct balloc_Chunk *chunk, size_t size) 
{
    size_t totalNeed = HEADER_SIZE + size;
    if (chunk->size < totalNeed) 
    {
        return -1;
    }


    if (chunk->size == totalNeed) {
        chunk->isAllocated = 1;
        return 1;
    }


    size_t leftoverSize = chunk -> size - totalNeed;

    chunk->isAllocated = 1;
    chunk->size = totalNeed;
    if (leftoverSize > HEADER_SIZE + ALIGNMENT) {
        struct balloc_Chunk *newChunnk = (struct balloc_Chunk *)((char *)chunk + totalNeed);
        newChunnk -> next = chunk->next;
        newChunnk->previous = chunk;
        newChunnk->size = leftoverSize;
        newChunnk->isAllocated = 0;

        if (chunk->next != NULL) {
            chunk->next->previous = newChunnk;
        }
        chunk->next = newChunnk;
    }
    else {
        chunk->size += leftoverSize;
    }

    return 1;
}

struct balloc_Chunk *startchunk;

void *balloc(size_t size) {
    if (size == 0)
    {
        return NULL;
    }
    size = Align(size);

    int found = 0;
    struct balloc_Chunk *last = NULL;

    if (startchunk != NULL) 
    {
        struct balloc_Chunk *curr = startchunk;

        while(curr != NULL)
        {
            if (!curr->isAllocated && curr->size >= size + HEADER_SIZE) {
                found = 1;
                break;
            }
            last = curr;
            curr = curr->next;
        }

        if (found) {
            int res = breakChunk(curr, size);
            if (res != 1) {
                return NULL;
            }

            return (void *)((char *)curr + HEADER_SIZE);
        }
    }

    size_t totalNeed = HEADER_SIZE + size;
    struct balloc_Chunk* chunk = sbrk(totalNeed);
    if (chunk == (void *)-1) {
        return NULL;
    }
    chunk->size = totalNeed;
    chunk->isAllocated = 1;
    chunk->next = NULL;

    if (startchunk == NULL) {
        chunk-> previous = NULL;
        startchunk = chunk;
    }
    else {
        chunk->previous = last;
        last->next = chunk;
    }

    return (void *)((char *)chunk + HEADER_SIZE);
}

void bfree(void* addr) {
    if (addr == NULL) {
        return;
    }

    struct balloc_Chunk* chunk = (struct balloc_Chunk *)((char *)addr - HEADER_SIZE);
    chunk->isAllocated= 0;
    if (chunk->next != NULL && !chunk->next->isAllocated) {
        chunk->size += chunk->next->size;
        chunk->next = chunk->next->next;
        if (chunk->next != NULL) {
            chunk->next->previous = chunk;
        }
    }
    if (chunk->previous != NULL && !chunk->previous->isAllocated) {
        chunk->previous->size += chunk->size;
        chunk->previous->next = chunk->next;
        if (chunk->next != NULL) {
            chunk->next->previous = chunk->previous;
        }
    }
}

void printHeap() {
    printf("printingHeap...\n");
    printf("=====================================\n");
    struct balloc_Chunk* curr = startchunk;
    if (curr == NULL) {
        printf("emptyHeap");
        printf("=====================================\n");
        return;
    }

    int index = 1;
    while(curr != NULL) {
        printf("%d\n", index);
        printf("address: %p\n", (void *) curr);
        printf("allocation: %s\n", curr->isAllocated? "[Allocated]" : "[NOT ALLOCATED]");
        printf("block size: %ld\n", curr->size);
        printf("payload size: %ld\n", curr->size - HEADER_SIZE);
        printf("=====\n");

        curr = curr->next;
        index ++;
    }
}