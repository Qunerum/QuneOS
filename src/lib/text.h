#ifndef TEXT_H
#define TEXT_H

#define MAX_STRING_LEN 512
#define MAX_STRING_INT_LEN 12

static inline int len(char* text) { char* start = text; while (*text) { text++; } return (int)(text - start); }
static inline int is(char* a, char* b) { while (*a != '\0' && *b != '\0') { if (*a != *b) return 0; a++; b++; } return (*a == *b); }

void addStr(char* a, char* b, int maxLen);
void addChar(char* a, char b, int maxLen);

#endif
