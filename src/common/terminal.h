#ifndef TERMINAL_H
#define TERMINAL_H

#include "../kernel/kernel.h"

void showCursor();
void hideCursor();

void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);
uint16_t inw(uint16_t port);
void outw(unsigned short port, uint16_t val);

#define HISTORY_MAX 16
#define CMD_MAX 128

void add_to_history(char* cmd);
void readLine(char* buffer, int maxLength);
void readPass(char* buffer, int maxLength);

void clear();
void printcb(char* msg, char clr, char bck);
void printb(char* msg, char back);
void printc(char* msg, char clr);
void print(char* msg);
void println(char* msg);
void printlne();

void printcbInt(int n, char clr, char bck);
void printbInt(int n, char back);
void printcInt(int n, char clr);
void printInt(int n);

void printcbHex(unsigned int n, char clr, char bck);
void printbHex(unsigned int n, char clr);
void printcHex(unsigned int n, char clr);
void printHex(unsigned int n);

void printq(char* format, ...);

#endif
