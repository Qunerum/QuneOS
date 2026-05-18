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

}
