#include "mouse.h"
#include "screen.h"
#include "keyboard.h"

#define MOUSE_SIZE_W 8
#define MOUSE_SIZE_H 8

volatile int mouseX = 0; volatile int mouseY = 0;
int oldMouseX = 0, oldMouseY = 0;
volatile int mouseUpdated = 0;
uint8_t mouseCycle = 0,
mousePacket[3];
uint32_t mouseBackBuffer[MOUSE_SIZE_W * MOUSE_SIZE_H];
const uint8_t mouse_pointer[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,0},
    {1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,0,0},
    {1,1,0,1,1,1,1,0},
    {1,0,0,0,1,1,1,1},
    {0,0,0,0,0,1,1,1},
    {0,0,0,0,0,0,1,1}
};
void mouse_handler_c() {
    uint8_t status = inb(0x64);
    if (status & 0x01) {
        uint8_t data = inb(0x60);

        switch(mouseCycle) {
            case 0:
                mousePacket[0] = data;
                if (!(data & 0x08)) {
                    break;
                }
                mouseCycle++;
                break;
            case 1:
                mousePacket[1] = data;
                mouseCycle++;
                break;
            case 2:
                mousePacket[2] = data;
                mouseCycle = 0;

                int rel_x = mousePacket[1];
                int rel_y = mousePacket[2];

                if (mousePacket[0] & 0x10) rel_x |= 0xFFFFFF00;
                if (mousePacket[0] & 0x20) rel_y |= 0xFFFFFF00;

                mouseX += rel_x;
            mouseY += rel_y;

            if (mouseX < -halfX) mouseX = -halfX;
            if (mouseX > halfX - MOUSE_SIZE_W) mouseX = halfX - MOUSE_SIZE_W;
            if (mouseY > halfY) mouseY = halfY;
            if (mouseY < -halfY + MOUSE_SIZE_H) mouseY = -halfY + MOUSE_SIZE_H;

            mouseUpdated = 1;
            break;
        }
    }

    outb(0xA0, 0x20); // Slave PIC
    outb(0x20, 0x20); // Master PIC
}
void mouse_restore_background() {
    int idx = 0;
    for (int y = 0; y < MOUSE_SIZE_H; y++) {
        for (int x = 0; x < MOUSE_SIZE_W; x++) {
            draw_pixel(oldMouseX + x, oldMouseY - y, mouseBackBuffer[idx++]);
        }
    }
}
void mouse_draw() {
    int idx = 0;
    for (int y = 0; y < MOUSE_SIZE_H; y++) { for (int x = 0; x < MOUSE_SIZE_W; x++) { mouseBackBuffer[idx++] = get_pixel(mouseX + x, mouseY - y); } }
    for (int y = 0; y < MOUSE_SIZE_H; y++) { for (int x = 0; x < MOUSE_SIZE_W; x++) { if (mouse_pointer[y][x] == 1) { draw_pixel(mouseX + x, mouseY - y, 0xFFFFFFFF); } } }
    oldMouseX = mouseX;
    oldMouseY = mouseY;
}
void mouse_wait(uint8_t type) { uint32_t timeout = 100000; if (type == 0) { while (timeout--) { if ((inb(0x64) & 2) == 0) return; } } else { while (timeout--) { if ((inb(0x64) & 1) == 1) return; } } }
uint8_t mouse_read(void) {
    mouse_wait(1);
    return inb(0x60);
}

void mouse_write(uint8_t write) {
    mouse_wait(0);
    outb(0x64, 0xD4);
    mouse_wait(0);
    outb(0x60, write);
}

uint32_t initMouse(void) {
    uint8_t status;

    mouse_wait(0);
    outb(0x64, 0xA8);

    mouse_wait(0);
    outb(0x64, 0x20);
    status = mouse_read();

    status |= 0x02;
    status &= ~0x20;

    mouse_wait(0);
    outb(0x64, 0x60);
    mouse_wait(0);
    outb(0x60, status);

    mouse_write(0xF6);
    mouse_read();

    mouse_write(0xF4);
    mouse_read();

    mouseX = 0;
    mouseY = 0;

    return 1;
}
