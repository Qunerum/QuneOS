#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "utils.h"

void run_menu_module(MenuModule* module) {
    int min = (strcmp(module->title, "QuneOS") == 0) ? 0 : -1;
    int selected = min;
    int running = 1;
    int current = 0;
    if (module->custom_render != NULL)
        min = 0;
    while (running) {
        clear_screen();

        if (module->custom_render != NULL) module->custom_render(module, current, selected); else
        {
            printf("+--- " YELLOW "%s " RESET, module->title);
            for (int i = 0; i < (17 - strlen(module->title)); i++)
                printf("-");
            printf("+\n");

            if (strcmp(module->title, "QuneOS") != 0)
            {
                if (selected == -1) printf(BG_WHITE RED " < Return " RESET "\n");
                else printf("   Return \n");
            }
            for (int i = 0; i < module->options_count; i++) {
                if (i == selected) printf(BG_BLUE WHITE " > %-20s " RESET "\n", module->options[i]);
                else printf("   %-20s \n", module->options[i]);
            }
        }
        current = getch();
        if (current == 'w' || current == 'A') { selected--; }
        else if (current == 's' || current == 'B') { selected++; }
        else if (current == 10) { if (selected == -1) { running = 0; continue; } else { module->on_select(selected); } }
        if (selected < min)
            selected = module->options_count - 1;
        else if (selected >= module->options_count)
            selected = min;
    }
}
