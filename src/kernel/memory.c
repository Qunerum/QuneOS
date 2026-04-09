#include "memory.h"

unsigned long free_mem_addr = 0x200000;

void* kmalloc(size_t size) {
    void* ptr = (void*)free_mem_addr;
    free_mem_addr += size;
    unsigned char* p = (unsigned char*)ptr;
    for(size_t i = 0; i < size; i++) {
        p[i] = 0;
    }

    return ptr;
}
unsigned long get_heap_pointer() {
    extern unsigned long free_mem_addr;
    return free_mem_addr;
}
