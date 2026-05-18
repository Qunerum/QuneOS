#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../lib/text.h"
#include "terminal.h"

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

    print("Welcome to QuneOS!\n", 0x00FF00);

    while(1) {
        print("QuneOS> ", 0xCCCCCC);

        setInput(1);

        while (inputEnable == 1) {
            __asm__ volatile("hlt");
        }

        char* cmd = getInput();

        if (len(cmd) == 0) {
            // Jeśli wcisnął sam Enter, nic nie rób
            continue;
        }
        else if (is(cmd, "help")) {
            print("Dostepne komendy: help, clear, version\n", 0x00FFFF);
        }
        else if (is(cmd, "version")) {
            print("QuneOS v0.2 (QCode Architecture)\n", 0x9999FF);
        }
        else if (is(cmd, "clear")) {
            clear();
        }
        else {
            print("Nieznana komenda: ", 0xFF0000);
            print(cmd, 0xFF0000);
            print("\n", 0xFF0000);
        }
    }
}
