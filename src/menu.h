#ifndef MENU_H
#define MENU_H

typedef struct MenuModule {
    char* title;
    char** options;
    int options_count;
    void (*on_select)(int option_index);
    void (*custom_render)(struct MenuModule* self, int c, int selected);
} MenuModule;

void run_menu_module(MenuModule* module);

extern MenuModule mainMenu;
extern MenuModule programsMenu;
// Programs / Apps
extern MenuModule fileApp;


extern MenuModule gamesMenu;

#endif
