#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../lib/text.h"
#include "terminal.h"

void printState(char* text, int state) {
    print(text, TEXT_COLOR);
    print(" [", DARK_GRAY);
    print(" OK ", DARK_GREEN);
    print("]\n", DARK_GRAY);
}

__attribute__((section(".text.prologue")))
void _start() {
    struct vbe_mode_info* vbe = (struct vbe_mode_info*)0x8000;
    if (vbe->physbase == 0) {
        vbe->physbase = 0xFD000000;
        vbe->pitch = 4096;
        vbe->bpp = 32;
        vbe->width = 1024;
        vbe->height = 768;
    }
    initScreen(vbe);
    initTerminal();
    initKeyboard();
    printState("Screen", 1);
    printState("Terminal", 1);
    printState("Keyboard", 1);

    print("\nWelcome to QuneOS!\n", GREEN);
    print("\x80", GRAY);
    print("\x80", RED);
    print("\x80", GREEN);
    print("\x80\n", BLUE);
    draw_text(0, 0, "\x80 \x81 \x82 \x83 \x84 \x85 \x86 \x87 \x88 \x89 \n", 2, TEXT_COLOR);

    while(1) {
        print("QuneOS \x81 ", TEXT_COLOR);

        setInput(1);

        while (inputEnable == 1) { __asm__ volatile("hlt"); }
        char* cmd = getInput();

        if (len(cmd) == 0) { continue; }
        else if (is(cmd, "help")) {
            print("Dostepne komendy: help, clear, version\n", YELLOW);
        }
        else if (is(cmd, "version")) {
            print("QuneOS v0.2 (QCode Architecture)\n", GREEN);
        }
        else if (is(cmd, "clear")) {
            clear();
        }
        else {
            print("Nieznana komenda: ", RED);
            print(cmd, RED);
            print("\n", RED);
        }
    }
}
