#ifndef KEYBOARD_H
#define KEYBOARD_H

// IDT
#include <stdint.h>
static inline uint8_t inb(uint16_t port) { uint8_t ret; __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port)); return ret; }
static inline void outb(uint16_t port, uint8_t val) { __asm__ volatile("outb %0, %1" :: "a"(val), "Nd"(port)); }
struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Keyboard
void initKeyboard();
extern int inputEnable;
void setInput(int state);
char* getInput();

#endif
