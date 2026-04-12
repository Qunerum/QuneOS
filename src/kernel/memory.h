// memory.h
#ifndef MEMORY_H
#define MEMORY_H

typedef unsigned int size_t;

typedef struct MemoryBlock {
    size_t size;
    int free;
    struct MemoryBlock* next;
} MemoryBlock;

void init_memory();
void* kmalloc(size_t size);
void kfree(void* ptr);
unsigned long get_heap_usage();

#endif
