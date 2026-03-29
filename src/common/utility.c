#include "../kernel/kernel.h"
#include "utility.h"

void delay(int count) { for (volatile int i = 0; i < count * 100000; i++) { } }
int len(char* str) {
    int len = 0;
    while (str[len] != '\0') { len++; }
    return len;
}
int is(char* a, char* b) {
    while (*a && (*a == *b)) { a++; b++; }
    int out = *(unsigned char*)a - *(unsigned char*)b;
    if (out == 0) return 1;
    return 0;
}
int startsWith(char* str, char* prefix) {
    while (*prefix) { if (*prefix++ != *str++) return 0; }
    return 1;
}
