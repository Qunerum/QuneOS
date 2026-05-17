#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
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
    initKeyboard();

    print("Hello!\n", 0xCCCCCC);
    print("Welcome to QuneOS!\nS\b\b\b\b", 0xCCCCCC);

    while(1);
}
