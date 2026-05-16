#include "../lib/math.h"
// #include "../lib/text.h"
#include "screen.h"
#include "font.h"

struct vbe_mode_info* vbe;
int ScreenX = 0, ScreenY = 0;
void initScreen(struct vbe_mode_info* v) { vbe = v; ScreenX = vbe->width / 2; ScreenY = vbe->height / 2; }
void calcPos(int x, int y, int* xo, int* yo) { *xo = ScreenX + x; *yo = ScreenY - y; }

void draw_pixel(int x, int y, uint32_t color) {
    calcPos(x, y, &x, &y);
    if (x < 0 || x >= vbe->width || y < 0 || y >= vbe->height) { return; }
    unsigned long pixel_offset = (y * vbe->pitch);

    if (vbe->bpp == 32) {
        uint32_t* pixel_ptr = (uint32_t*)((unsigned long)vbe->physbase + pixel_offset + (x * 4));
        *pixel_ptr = color;
    }
    else if (vbe->bpp == 24) {
        unsigned char* pixel_ptr = (unsigned char*)((unsigned long)vbe->physbase + pixel_offset + (x * 3));
        pixel_ptr[0] = color & 0xFF;         // Blue
        pixel_ptr[1] = (color >> 8) & 0xFF;  // Green
        pixel_ptr[2] = (color >> 16) & 0xFF; // Red
    }
    else if (vbe->bpp == 16) {
        unsigned short* pixel_ptr = (unsigned short*)((unsigned long)vbe->physbase + pixel_offset + (x * 2));
        unsigned short r = (color >> 16) & 0xFF;
        unsigned short g = (color >> 8) & 0xFF;
        unsigned short b = color & 0xFF;
        *pixel_ptr = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }
}
void draw_line(int x1, int y1, int x2, int y2, int thickness, uint32_t color) {
    int dx = absolute(x2 - x1);
    int dy = -absolute(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    while (1) {
        if (thickness <= 1) { draw_pixel(x1, y1, color); } else {
            int offset = thickness / 2;
            for (int ty = -offset; ty <= offset; ty++) {
                for (int tx = -offset; tx <= offset; tx++) { draw_pixel(x1 + tx, y1 + ty, color); } }
        }
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}
void draw_circle(int x, int y, int r, int thickness, int startAngle, int endAngle, uint32_t color) {
    int oldX, oldY;
    int first = 1;
    while(startAngle < 0) startAngle += 360;
    while(endAngle < startAngle) endAngle += 360;
    for (int a = startAngle; a <= endAngle; a += 15) {
        int cos_val, sin_val;
        get_circle_coords(a % 360, &cos_val, &sin_val);
        int px = x + (r * cos_val) / 100;
        int py = y + (r * sin_val) / 100;
        if (!first) { draw_line(oldX, oldY, px, py, thickness, color); } else { first = 0; }
        oldX = px;
        oldY = py;
    }
}

void draw_char(int x, int y, char c, uint32_t color, int scale) {
    const unsigned char* bitmap = font_basic[(unsigned char)c];
    for (int row = 0; row < 8; row++) { for (int col = 0; col < 8; col++) {
            if (bitmap[row] & (0x80 >> col)) { for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) { draw_pixel(x + (col * scale) + sx, y - (row * scale) - sy, color); } } } } }
}
void draw_text(int x, int y, char* str, int scale, uint32_t color) {
    int sx = x;
    while (*str) {
        if (*str == '\n') { y -= (8 * scale); x = sx; str++; continue; }
        draw_char(x, y, *str, color, scale);
        x += (8 * scale);
        str++;
    }
}
