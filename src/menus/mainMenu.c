#include <sys/reboot.h>
#include <unistd.h>
#include "../menu.h"

static void handle(int index, MenuModule* me)
{
    switch (index)
    {
        case 0: run_menu_module(&programsMenu); break;
        case 1: run_menu_module(&gamesMenu); break;
        case 2: me->running = 0; break;
    }
}
static char* opts[] = { "Programs", "Games", "Quit" };
MenuModule mainMenu = {
    .title = "QuneOS",
    .options = opts,
    .options_count = 3,
    .running = 1,
    .on_select = handle
};
