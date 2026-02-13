#include <stdio.h>
#include "utils.h"

void clear_screen() {
    printf("\e[1;1H\e[2J");
}

void print_color(const char* text, const char* color_code) {
    printf("%s%s%s", color_code, text, RESET);
}
