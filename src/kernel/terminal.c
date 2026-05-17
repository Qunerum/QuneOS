#include "../drivers/screen.h"
#include "../lib/text.h"

int cursorX, cursorY;
int maxX = 128, maxY = 20;

void print(char* text, uint32_t color) {
    int l = len(text);
    while(*text) {
        if (*text == '\n') { cursorX = 0; cursorY++; text++; continue; }
        if (*text == '\b' && cursorX > 0) { cursorX--; draw_rect_fill(-ScreenX + (cursorX + 1) * 8 + 4, ScreenY - cursorY * 8 - 11, 8, 8, 0x000000); text++; continue; }
        cursorX++;
        draw_char(-ScreenX + cursorX * 8, ScreenY - cursorY * 8 - 8, *text, color, 1);
        text++;
    }
}
