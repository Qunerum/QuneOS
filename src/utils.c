#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "utils.h"

int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Wyłącz buforowanie i wyświetlanie wpisanych znaków
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
void clear_screen() {
    printf("\e[1;1H\e[2J");
}

void print_color(const char* text, const char* color_code) {
    printf("%s%s%s", color_code, text, RESET);
}

