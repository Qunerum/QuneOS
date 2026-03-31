#ifndef KERNEL_H
#define KERNEL_H

// COLORS
#define BLACK          0x00
#define BLUE           0x01
#define GREEN          0x02
#define CYAN           0x03
#define RED            0x04
#define MAGENTA        0x05
#define BROWN          0x06
#define LIGHT_GRAY     0x07
#define DARK_GRAY      0x08
#define LIGHT_BLUE     0x09
#define LIGHT_GREEN    0x0A
#define LIGHT_CYAN     0x0B
#define LIGHT_RED      0x0C
#define LIGHT_MAGENTA  0x0D
#define YELLOW         0x0E
#define WHITE          0x0F
#define MKCLR(fg, bg) ((bg << 4) | fg)


typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;

typedef unsigned long uintptr_t;

#endif
