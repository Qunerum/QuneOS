#include "../drivers/screen.h"
#include "../drivers/font.h"

int cursorX = 0, cursorY = 0;
int maxX = 128, maxY = 80;
int scale = 1;
uint32_t initTerminal() {
    if (!screenX || !screenY) { return 0; }
    maxX = screenX / CHAR_SIZE;
    maxY = screenY / (CHAR_SIZE + 1);
    return 0x1000;
}
void printChar(char c, uint32_t color) {
    if (c == '\n') { cursorX = 0; cursorY++; } else {
        int x = -halfX + (cursorX + 1) * CHAR_SIZE, y = halfY - (cursorY + 1) * (CHAR_SIZE + 1);
        if ((c == '\b') && cursorX > 0) { cursorX--; draw_char(x, y, '\x80', BACKGROUND_COLOR, scale); } else { cursorX++; draw_char(x + CHAR_SIZE, y, c, color, scale); } }
}
void print(char* text, uint32_t color) { while(*text) { printChar(*text, color); text++; } }
