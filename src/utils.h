#ifndef UTILS_H
#define UTILS_H

#define RESET       "\033[0m"
#define UNDERLINE   "\033[4m"

#define RED         "\033[1;31m"
#define GREEN       "\033[1;32m"
#define YELLOW      "\033[1;33m"
#define BLUE        "\033[1;34m"
#define PINK        "\033[1;35m"
#define CYAN        "\033[1;36m"
#define WHITE       "\033[1;37m"

#define BG_BLUE     "\033[44m"
#define BG_WHITE    "\033[47m"
#define BG_CYAN     "\033[46m"

int getch(void);
void clear_screen();
void print_color(const char* text, const char* color_code);

#endif
