#ifndef UTILITY_H
#define UTILITY_H

void delay(int count);
int len(char* str);
int is(char* a, char* b);
int startsWith(char* str, char* prefix);
char* addStr(char* a, char* b);
char* intToStr(int n);
int strToInt(char* str);
int fndFirst(char* a, char b);
int fndLast(char* a, char b);
char* cutStart(char* str, int n);
char* cutEnd(char* str, int n);
void copyStr(char* target, char* source);
void split(char* input, char delimiter, char* outA, char* outB);

#endif
