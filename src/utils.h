#ifndef UTILS_H
#define UTILS_H

void clear_screen();
void print_color(const char* text, const char* color_code);

// Kody kolorów ANSI
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

#endif
