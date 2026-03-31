#include "../kernel/kernel.h"
#include "terminal.h"
#include "utility.h"

static char* vga_buffer = (char*)0xB8000;
static int cursorX;
static int cursorY;
static int cursor = 1;

void outb(unsigned short port, unsigned char val) { asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) ); }
unsigned char inb(unsigned short port) { unsigned char ret; asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port)); return ret; }
uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void update_cursor() {
    unsigned short pos = cursorY * 80 + cursorX;
    if (!cursor) { pos = 4000; }
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}
void showCursor() { cursor = 1; update_cursor(); }
void hideCursor() { cursor = 0; update_cursor(); }
void scroll_screen() {
    for (int i = 0; i < 24 * 80 * 2; i++) { vga_buffer[i] = vga_buffer[i + 160]; }
    for (int i = 24 * 80 * 2; i < 25 * 80 * 2; i += 2) {
        vga_buffer[i] = ' ';
        vga_buffer[i + 1] = 0x07;
    }
    cursorY = 24;
}

unsigned char kbd_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};
unsigned char kbd_map_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};
static int shift_pressed = 0;
char get_char() {
    while (!(inb(0x64) & 1));
    unsigned char scancode = inb(0x60);
    if (scancode == 0x2A || scancode == 0x36) { shift_pressed = 1; return 0; }
    if (scancode == 0xAA || scancode == 0xB6) { shift_pressed = 0; return 0; }
    if (scancode & 0x80) return 0;
    if (scancode == 0x48 || scancode == 0x50) { return (char)scancode; }
    if (shift_pressed) { return kbd_map_shift[scancode]; } else { return kbd_map[scancode]; }
}

static char history[HISTORY_MAX][CMD_MAX];
static int history_count = 0;
static int history_index = -1;
static int current_history_pos = 0;
void add_to_history(char* cmd) {
    if (len(cmd) == 0) return;
    int i = 0;
    while (cmd[i] != '\0' && i < CMD_MAX - 1) {
        history[current_history_pos][i] = cmd[i];
        i++;
    }
    history[current_history_pos][i] = '\0';
    current_history_pos = (current_history_pos + 1) % HISTORY_MAX;
    if (history_count < HISTORY_MAX) {
        history_count++;
    }
}
void readLine(char* buffer, int maxLength) {
    int i = 0;
    showCursor();
    int temp_history_index = -1;
    while (i < maxLength - 1) {
        char c = get_char();
        if (c == 0) continue;
        if (c == 0x48 || c == 0x50) {
            if (history_count == 0) continue;
            if (c == 0x48) { if (temp_history_index < history_count - 1) temp_history_index++; }
            else { if (temp_history_index > -1) temp_history_index--; }

            while (i > 0) { i--; cursorX--; print(" "); cursorX--; }
            if (temp_history_index == -1) { buffer[0] = '\0'; i = 0; } else {
                int pos = (current_history_pos - 1 - temp_history_index + HISTORY_MAX) % HISTORY_MAX;
                char* h_cmd = history[pos];
                for (i = 0; h_cmd[i] != '\0' && i < maxLength - 1; i++) {
                    buffer[i] = h_cmd[i];
                    char s[2] = {buffer[i], '\0'};
                    print(s);
                }
                buffer[i] = '\0';
            }
            update_cursor();
            continue;
        }
        if (c == '\n') {  print("\n"); break; }
        if (c == '\b') {
            if (i > 0) {
                i--;
                cursorX--;
                print(" ");
                cursorX--;
                update_cursor();
            }
            continue;
        }
        temp_history_index = -1;
        buffer[i++] = c;
        char s[2] = {c, '\0'};
        print(s);
    }
    buffer[i] = '\0';
}

void clear()
{
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        vga_buffer[i] = ' ';
        vga_buffer[i + 1] = 0x07;
    }
    cursorX = 1;
    cursorY = 0;
    update_cursor();
}
void printcb(char* msg, char clr, char bck)
{
    for (int i = 0; msg[i] != '\0'; i++) {
        if (cursorY >= 25) { scroll_screen(); }
        if (msg[i] == '\n') { cursorX = 1; cursorY++; continue; }
        if (cursorX >= 80) { cursorX = 1; cursorY++; }
        int l = (cursorY * 80 + cursorX) * 2;
        vga_buffer[l] = msg[i];
        vga_buffer[l + 1] = MKCLR(clr, bck);
        cursorX++;
    }
    update_cursor();
}
void printb(char* msg, char clr) { printcb(msg, LIGHT_GRAY, clr); }
void printc(char* msg, char clr) { printcb(msg, clr, BLACK); }
void print(char* msg) { printc(msg, LIGHT_GRAY); }

void printcbInt(int n, char clr, char bck) {
    if (n == 0) { printcb("0", clr, bck); return; }
    char buf[18];
    int i = 16;
    buf[17] = '\0';
    unsigned int num = n;
    if (n < 0) {
        printcb("-", clr, bck);
        num = -n;
    }
    while (num > 0) {
        buf[i--] = (num % 10) + '0';
        num /= 10;
    }
    printcb(&buf[i + 1], clr, bck);
}
void printbInt(int n, char clr) { printcbInt(n, LIGHT_GRAY, clr); }
void printcInt(int n, char clr) { printcbInt(n, clr, BLACK); }
void printInt(int n) { printcbInt(n, LIGHT_GRAY, BLACK); }

void printcbHex(unsigned int n, char clr, char bck) {
    char* hexChars = "0123456789ABCDEF";
    char buf[11];
    buf[0] = '0';
    buf[1] = 'x';
    buf[10] = '\0';

    for (int i = 9; i >= 2; i--) {
        buf[i] = hexChars[n & 0xF];
        n >>= 4;
    }
    printcb(buf, clr, bck);
}

void printbHex(unsigned int n, char clr) { printcbHex(n, LIGHT_GRAY, clr); }
void printcHex(unsigned int n, char clr) { printcbHex(n, clr, BLACK); }
void printHex(unsigned int n) { printcbHex(n, LIGHT_GRAY, BLACK); }

void printq(char* format, ...) {
    char** argPtr = (char**)&format;
    argPtr++;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
                case 's': {
                    char* s = (char*)*argPtr;
                    print(s);
                    argPtr++;
                    break;
                }
                case 'd': {
                    int d = (uintptr_t)*argPtr;
                    printInt(d);
                    argPtr++;
                    break;
                }
                case 'x': {
                    unsigned int x = (uintptr_t)*argPtr;
                    printHex(x);
                    argPtr++;
                    break;
                }
                case 'c': {
                    char c = (uintptr_t)*argPtr;
                    char str[2] = {c, '\0'};
                    print(str);
                    argPtr++;
                    break;
                }
                case '%': { print("%"); break; }
            }
        } else { char str[2] = {format[i], '\0'}; print(str); }
    }
}

