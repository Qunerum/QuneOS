#ifndef TERMINAL_H
#define TERMINAL_H

typedef unsigned int uint32_t;
uint32_t initTerminal();
void printChar(char c, uint32_t color);
void print(char* text, uint32_t color);

#endif
