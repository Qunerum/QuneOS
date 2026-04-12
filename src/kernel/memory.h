#ifndef MEMORY_H
#define MEMORY_H

typedef unsigned int size_t;

// Struktura opisująca każdy blok pamięci na stercie
typedef struct MemoryBlock {
    size_t size;            // Rozmiar dostępnej pamięci (bez nagłówka)
    int free;               // 1 jeśli wolny, 0 jeśli zajęty
    struct MemoryBlock* next; // Wskaźnik na następny blok
} MemoryBlock;

void init_memory();         // Musisz to wywołać raz w kmain!
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
