#include "../kernel/terminal.h"
#include "../kernel/memory.h"
#include "../lib/text.h"
#include "keyboard.h"

// IDT
struct idt_entry idt[256];
struct idt_ptr idtp;
extern void idt_load(uint32_t pointer);
extern void keyboard_handler_asm(void);
void set_idt_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}
void init_pic(void) {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}
void init_idt(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;
    for(int i = 0; i < 256; i++) { set_idt_gate(i, 0, 0, 0); }
    init_pic();
    set_idt_gate(0x21, (uint32_t)keyboard_handler_asm, 0x08, 0x8E);
    idt_load((uint32_t)&idtp);
    __asm__ volatile("sti");
}

// Keyboard
static const char scancode_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',   0, ' '
};
char* input = NULL;
void initKeyboard() { init_idt(); input = (char*)kmalloc(512); }
int inputEnable = 0;
void setInput(int state) { inputEnable = state; if (state) { input[0] = '\0'; } }
char* getInput() { return input; }
uint32_t color = 0xCCCCCC;
void keyboard_handler_c(void) {
    uint8_t scancode = inb(0x60);
    if (inputEnable && scancode < 0x80) {
        char c = scancode_ascii[scancode];
        if (c != 0) {
            if (c == '\n') { inputEnable = 0; printChar('\n', color); }
            else if (c == '\t') { print("    ", color); addStr(input, "    ", 512); }
            else if (c == '\b') { int l = len(input); if (l > 0) { input[l - 1] = '\0'; printChar('\b', color); } }
            else { printChar(c, color); addChar(input, c, 512); }
        }
    }
    outb(0x20, 0x20);
}
