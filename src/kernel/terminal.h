#ifndef TERMINAL_H
#define TERMINAL_H

#include "../kernel/types.h" // IWYU pragma: keep

uint32_t initTerminal();
void printChar(char c, uint32_t color);
void print(char* text, uint32_t color);

#endif
