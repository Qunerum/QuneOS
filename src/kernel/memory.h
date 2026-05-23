#ifndef MEMORY_H
#define MEMORY_H

#include "types.h" // IWYU pragma: keep

typedef struct MemoryBlock {
    size_t size;
    int free;
    struct MemoryBlock* next;
} MemoryBlock;

void init_memory();
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
