#include "../kernel/kernel.h"
#include "utility.h"


// < = = = = = = = = = = > TERMINAL < = = = = = = = = = = >
static char* vga_buffer = (char*)0xB8000;
static int cursorX;
static int cursorY;
static int cursor = 1;


void outb(unsigned short port, unsigned char val) { asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) ); }
void update_cursor() {
    unsigned short pos = cursorY * 80 + cursorX;
    if (!cursor) { pos = 6425; }
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}
void showCursor() { cursor = 1; update_cursor(); }
void hideCursor() { cursor = 0; update_cursor(); }

void clear()
{
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        vga_buffer[i] = ' ';
        vga_buffer[i + 1] = 0x07;
    }
    cursorX = 0;
    cursorY = 0;
    update_cursor();
}
void printcb(char* msg, char clr, char bck)
{
    for (int i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') { cursorX = 0; cursorY++; continue; }
        if (cursorX >= 80) { cursorX = 0; cursorY++; }
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

// < = = = = = = = = = = > END TERMINAL < = = = = = = = = = = >
