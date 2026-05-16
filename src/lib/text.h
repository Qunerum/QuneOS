#ifndef TEXT_H
#define TEXT_H

static inline int len(char* text) { char* start = text; while (*text) { text++; } return (int)(text - start); }
static inline int is(char* a, char* b) { while (*a != '\0' && *b != '\0') { if (*a != *b) return 0; a++; b++; } return (*a == *b); }

#endif
