#include <stdio.h>
#include <sys/reboot.h>
#include <unistd.h>
#include "utils.h"

int main() {
    int selected = 0;
    int running = 1;

    const char* options[] = {
        "Programs",
        "Games",
        "Quit"
    };
    int max = sizeof(options) / sizeof(options[0]);

    while (running) {
        clear_screen();

        printf("+-- QuneOs ----------+\n");

        for (int i = 0; i < max; i++)
            printf(" %s %s\n" RESET, (selected == i) ? GREEN ">" : " ", options[1]);

        printf("\n(Use W/S or Arrows)\n");

        int c = getch();

        if (c == 'w' || c == 'W') { selected--; } else if (c == 's' || c == 'S') { selected++; }
        else if (c == '\033') { getch(); int arrow = getch(); if (arrow == 'A') { selected--; } else if (arrow == 'B') { selected++; } }
        else if (c == 10) // Enter
        {
            switch (selected) {
                case 0:

                    break;

                case 1:

                    break;

                case 2:
                    clear_screen();
                    print_color("Shutting down QuneOs...\n", RED);
                    sync();
                    reboot(RB_POWER_OFF);
                    break;
            }
        }

        if (selected >= max)
            selected = 0;
        else if (selected < 0)
            selected = max - 1;
    }

    printf("Exiting QuneOs...\n");
    return 0;
}
