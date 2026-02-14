#include <stdio.h>
#include <sys/reboot.h>
#include <unistd.h>
#include "../menu.h"

static void handle(int index)
{
    switch (index)
    {
        case 0: run_menu_module(&programsMenu); break;
        case 1: run_menu_module(&gamesMenu); break;
        case 2:
            printf("Exiting...\n");
            sync();
            reboot(RB_POWER_OFF);
            break;
    }
}
static char* opts[] = { "Programs", "Games", "Quit" };

MenuModule mainMenu = {
    .title = "QuneOS",
    .options = opts,
    .options_count = 3,
    .on_select = handle
};
