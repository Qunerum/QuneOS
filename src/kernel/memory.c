#include "memory.h"

#define HEAP_SIZE  0x1000000 // 16MB

#define ALIGN(size) (((size) + 7) & ~7)

static char heap_memory[HEAP_SIZE];
MemoryBlock* freeList = (MemoryBlock*)heap_memory;

void init_memory() {
    freeList->size = HEAP_SIZE - sizeof(MemoryBlock);
    freeList->free = 1;
    freeList->next = 0;
}

void* kmalloc(size_t size) {
    size = ALIGN(size);

    MemoryBlock* curr = freeList;
    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size > size + sizeof(MemoryBlock) + 8) {
                MemoryBlock* nextBlock = (MemoryBlock*)((char*)curr + sizeof(MemoryBlock) + size);

                nextBlock->size = curr->size - size - sizeof(MemoryBlock);
                nextBlock->free = 1;
                nextBlock->next = curr->next;

                curr->size = size;
                curr->next = nextBlock;
            }
            curr->free = 0;

            unsigned char* p = (unsigned char*)((char*)curr + sizeof(MemoryBlock));
            for(size_t i = 0; i < size; i++) p[i] = 0;

            return (void*)p;
        }
        curr = curr->next;
    }
    return 0; // Out of memory!
}

void kfree(void* ptr) {
    if (!ptr) return;

    MemoryBlock* block = (MemoryBlock*)((char*)ptr - sizeof(MemoryBlock));
    block->free = 1;

    MemoryBlock* curr = freeList;
    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += curr->next->size + sizeof(MemoryBlock);
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}
