#ifndef UTILITY_H
#define UTILITY_H

void showCursor();
void hideCursor();

void clear();
void printcb(char* msg, char clr, char bck);
void printb(char* msg, char back);
void printc(char* msg, char clr);
void print(char* msg);

void printcbInt(int n, char clr, char bck);
void printbInt(int n, char back);
void printcInt(int n, char clr);
void printInt(int n);

void delay(int count);

#endif
