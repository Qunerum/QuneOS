#include <sys/reboot.h>
#include <unistd.h>
#include "../menu.h"

static void handle(int index, MenuModule* me)
{
    switch (index)
    {
        case 0:
            run_menu_module(&fileApp);
            break;
        case 1:

            break;
    }
}
static char* opts[] = { "Files", "Calculator" };

MenuModule programsMenu = {
    .title = "Programs",
    .options = opts,
    .options_count = 2,
    .running = 1,
    .on_select = handle
};
