#include "../drivers/screen.h"
// #include "../lib/math.h"

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

    draw_circle(0, 0, 150, 2, -90, 0, 0x00FF00);
    draw_text(0, 0, "ABa", 0xCCCCCC, 2);

    while(1);
}
