#include "../kernel/terminal.h"
#include "../kernel/types.h" // IWYU pragma: keep
// #include "../kernel/memory.h"
#include "../lib/text.h"
#include "keyboard.h"
#include "screen.h"

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
static const char scancode_ascii_shift[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',   0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0, '*',   0, ' '
};

char input[MAX_STRING_LEN];
uint32_t initKeyboard() {
    init_idt();
    return (uint32_t)input;
}
int inputEnable = 0;
void setInput(int state) { inputEnable = state; if (state) { input[0] = '\0'; } }
int is_shift_pressed = 0, is_caps_lock_active = 0;
char* getInput() { return input; }
void keyboard_handler_c(void) {
    uint8_t scancode = inb(0x60);
    if (scancode == 0x2A || scancode == 0x36) { is_shift_pressed = 1; }
    else if (scancode == 0xAA || scancode == 0xB6) { is_shift_pressed = 0; }
    else if (scancode == 0x3A) { is_caps_lock_active = !is_caps_lock_active; }
    else if (inputEnable && scancode < 0x80) {
        char c = 0;
        if (is_shift_pressed) { c = scancode_ascii_shift[scancode]; } else { c = scancode_ascii[scancode]; }
        if (c != 0) {
            if (is_caps_lock_active && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) { if (c >= 'a' && c <= 'z') { c -= 32; } else if (c >= 'A' && c <= 'Z') { c += 32; } }
            if (c == '\n') { inputEnable = 0; printChar('\n', TEXT_COLOR); }
            else if (c == '\t') { print("    ", TEXT_COLOR); addStr(input, "    ", MAX_STRING_LEN); }
            else if (c == '\b') { int l = len(input); if (l > 0) { input[l - 1] = '\0'; printChar('\b', BACKGROUND_COLOR); } }
            else { printChar(c, INPUT_COLOR); addChar(input, c, MAX_STRING_LEN); }
        }
    }
    outb(0x20, 0x20);
}
