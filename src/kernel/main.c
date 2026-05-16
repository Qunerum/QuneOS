#include "../drivers/screen.h"

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

    draw_text(-ScreenX + 10, ScreenY - 10, "Welcome to QuneOS!", 1, 0xCCCCCC);

    while(1);
}
