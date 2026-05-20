#include "text.h"

void addStr(char* a, char* b, int maxLen) {
    int al = len(a), i = 0;
    if (al >= maxLen - 1) { if (maxLen > 0) a[maxLen - 1] = '\0'; return; }
    while (b[i]) {
        if (al + i >= maxLen - 1) { a[maxLen - 1] = '\0'; return; }
        a[al + i] = b[i];
        i++;
    }
    a[al + i] = '\0';
}
void addChar(char* a, char b, int maxLen) {
    int al = len(a);
    if (al >= maxLen - 1) { if (maxLen > 0) a[maxLen - 1] = '\0'; return; }
    a[al] = b; a[al + 1] = '\0';
}

void intToStr(int v, char* out) {
    int i = 0;
    unsigned int num;
    if (v == 0) {
        out[i++] = '0';
        out[i] = '\0';
        return;
    }
    if (v < 0) {
        out[i++] = '-';
        if (v == -2147483648) {
            out[i++] = '2'; out[i++] = '1'; out[i++] = '4'; out[i++] = '7';
            out[i++] = '4'; out[i++] = '8'; out[i++] = '3'; out[i++] = '6';
            out[i++] = '4'; out[i++] = '8'; out[i++] = '\0';
            return;
        }
        num = -v;
    } else { num = v; }
    int digits_start = i;
    while (num > 0) {
        out[i++] = (num % 10) + '0';
        num /= 10;
    }
    out[i] = '\0';
    int start = digits_start, end = i - 1;
    while (start < end) {
        char temp = out[start];
        out[start] = out[end];
        out[end] = temp;
        start++;
        end--;
    }
}
void intToHex(uint32_t v, char* out) {
    int i = 0;
    if (v == 0) { copyStr(out, "0x00000000"); return; }
    while (v > 0) {
        int rem = v & 0xF;
        if (rem < 10) { out[i++] = rem + '0'; } else { out[i++] = (rem - 10) + 'A'; }
        v = v >> 4;
    }
    while (i < 8) { out[i++] = '0'; }
    out[i++] = 'x';
    out[i++] = '0';
    out[i] = '\0';
    int start = 0, end = i - 1;
    while (start < end) {
        char temp = out[start];
        out[start] = out[end];
        out[end] = temp;
        start++;
        end--;
    }
}
void splitStart(char* in, char delimiter, char* outA, char* outB, int maxLen) {
    int i = 0, ia = 0, ib = 0, found = 0;
    outA[0] = '\0';
    outB[0] = '\0';
    if (in == 0 || in[0] == '\0' || maxLen <= 0) return;
    while (in[i] != '\0') {
        if (in[i] == delimiter && !found) { found = 1; i++; continue; }
        if (!found) { if (ia < maxLen - 1) { outA[ia++] = in[i]; } } else { if (ib < maxLen - 1) { outB[ib++] = in[i]; } }
        i++;
    }
    outA[ia] = '\0';
    outB[ib] = '\0';
}
void copyStr(char* target, char* source) { if (!target || !source) return; int i = 0; while (source[i] != '\0') { target[i] = source[i]; i++; } target[i] = '\0'; }
