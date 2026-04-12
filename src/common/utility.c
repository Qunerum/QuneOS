#include "../kernel/kernel.h"
#include "utility.h"

void delay(int count) { for (volatile int i = 0; i < count * 100000; i++) { } }
int len(char* str) {
    int len = 0;
    while (str[len] != '\0') { len++; }
    return len;
}
int leni(char* str, char ignore) {
    int i = 0;
    int len = 0;
    while (str[i] != '\0') { if (str[i] != ignore) { len++; } i++; }
    return len;
}
int is(char* a, char* b) {
    // if (len(a) != len(b)) return 0;
    while (*a && (*a == *b)) { a++; b++; }
    int out = *(unsigned char*)a - *(unsigned char*)b;
    if (out == 0) return 1;
    return 0;
}
int startsWith(char* str, char* prefix) {
    while (*prefix) { if (*prefix++ != *str++) return 0; }
    return 1;
}
static char result_buffer[STR_LEN];
char* addStr(char* a, char* b) {
    int i = 0, j = 0;
    while (a[i] != '\0' && i < STR_LEN - 1) { result_buffer[i] = a[i]; i++; }
    while (b[j] != '\0' && i < STR_LEN - 1) { result_buffer[i] = b[j]; i++; j++; }
    result_buffer[i] = '\0';
    return result_buffer;
}
char* intToStr(int n) {
    static char buf[18];
    if (n == 0) return "0";
    int i = 16;
    buf[17] = '\0';
    unsigned int num = (n < 0) ? -n : n;
    while (num > 0) { buf[i--] = (num % 10) + '0'; num /= 10; }
    if (n < 0) { buf[i--] = '-'; }
    return &buf[i + 1];
}
int strToInt(char* str) {
    int res = 0;
    int sign = 1;
    int i = 0;
    while (str[i] == ' ') { i++; }
    if (str[i] == '-') { sign = -1; i++; }
    while (str[i] >= '0' && str[i] <= '9') { res = res * 10 + (str[i] - '0'); i++; }
    return res * sign;
}
int fndFirst(char* a, char b) { for (int i = 0; i < len(a); i++) { if (a[i] == b) { return i; } } return -1; }
int fndLast(char* a, char b) { for (int i = len(a) - 1; i >= 0; i--) { if (a[i] == b) { return i; } } return -1; }
char* cutStart(char* str, int n) {
    int l = len(str);
    if (n >= l) return "";
    return str + n;
}
char* cutEnd(char* str, int n) {
    int l = len(str);
    if (n >= l) { str[0] = '\0'; return str; }
    str[l - n] = '\0';
    return str;
}
void copyStr(char* target, char* source) {
    int i = 0;
    while (source[i] != '\0') { target[i] = source[i]; i++; }
    target[i] = '\0';
}
int contains(char* input, char whot) { int i = 0;  while (input[i] != '\0') { if (input[i] == whot) return 1; i++; } return 0; }
void split(char* input, char delimiter, char* outA, char* outB) {
    int i = 0, ia = 0, ib = 0, found = 0;
    outA[0] = '\0';
    outB[0] = '\0';
    if (input == 0 || input[0] == '\0') return;
    while (input[i] != '\0') {
        if (input[i] == delimiter && !found) { found = 1; }
        else if (!found) { if (ia < STR_LEN - 1) outA[ia++] = input[i]; }
        else { if (ib < STR_LEN - 1) outB[ib++] = input[i]; }
        i++;
    }
    outA[ia] = '\0';
    outB[ib] = '\0';
}
