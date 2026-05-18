#ifndef SCREEN_H
#define SCREEN_H

#define BLACK           0x000000
// Normal
#define GRAY            0x888888
#define RED             0xEE0000
#define GREEN           0x00EE00
#define BLUE            0x0000EE
#define MAGENTA         0xEE00EE
#define ORANGE          0xEE8800
// Light
#define WHITE           0xFFFFFF
#define CARMINE         0xEE4444
#define LIME            0x44EE44
#define CYAN            0x00EEEE
#define PINK            0xEE44EE
#define YELLOW          0xEEEE00
// Dark
#define DARK_GRAY       0x444444
#define DARK_RED        0x880000
#define DARK_GREEN      0x008800
#define DARK_BLUE       0x000088
#define PURPLE          0x880088
#define BROWN           0x444400

#define BACKGROUND_COLOR BLACK
#define TEXT_COLOR GRAY

struct vbe_mode_info {
    unsigned short attributes;
    unsigned char win_a, win_b;
    unsigned short granularity;
    unsigned short winsize;
    unsigned short segment_a, segment_b;
    unsigned int win_func_ptr;
    unsigned short pitch;
    unsigned short width;
    unsigned short height;
    unsigned char char_width, char_height, planes, bpp, banks;
    unsigned char memory_model, bank_size, image_pages;
    unsigned char reserved0;
    unsigned char red_mask, red_position;
    unsigned char green_mask, green_position;
    unsigned char blue_mask, blue_position;
    unsigned char reserved_mask, reserved_position;
    unsigned char direct_color_attributes;
    unsigned int physbase;
} __attribute__((packed));
typedef unsigned int uint32_t;

extern int screenX;
extern int screenY;
extern int halfX;
extern int halfY;

void initScreen(struct vbe_mode_info* v);
void clear();

void draw_pixel(int x, int y, uint32_t color);
void draw_line(int x1, int y1, int x2, int y2, int thickness, uint32_t color);

// Wire
void draw_rect(int x, int y, int w, int h, int thickness, uint32_t color);
void draw_circle(int cx, int cy, int r, int thickness, int startAngle, int endAngle, uint32_t color);

// Fill
void draw_rect_fill(int x, int y, int w, int h, uint32_t color);

// Text
void draw_char(int x, int y, char c, uint32_t color, int scale);
void draw_text(int x, int y, char* str, int scale, uint32_t color);

#endif
