#include "../drivers/screen.h"
#include "../drivers/font.h"
#include "../lib/text.h"

int cursorX, cursorY;
int maxX = 128, maxY = 80;
void initTerminal() { maxX = screenX / CHAR_SIZE; maxY = screenY / (CHAR_SIZE + 1); }
void printChar(char c, uint32_t color) {
    if (c == '\n') { cursorX = 0; cursorY++; } else
        if (c == '\b' && cursorX > 0) { draw_rect_fill(-halfX + cursorX * CHAR_SIZE + 4, halfY - (cursorY - 1) * (CHAR_SIZE + 1) - CHAR_SIZE / 2 + 1, CHAR_SIZE, CHAR_SIZE, 0x000000); cursorX--; } else
        { cursorX++; draw_char(-halfX + cursorX * CHAR_SIZE, halfY - (cursorY - 1) * (CHAR_SIZE + 1), c, color, 1); }
}
void print(char* text, uint32_t color) {
    int l = len(text);
    while(*text) {
        printChar(*text, color);
        text++;
    }
}
