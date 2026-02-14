#include <stdio.h>
#include <sys/reboot.h>
#include <unistd.h>
#include "../menu.h"

static void handle(int index)
{
    switch (index)
    {
        case 0:

            break;
        case 1:

            break;
        case 2:
            printf("Exiting...\n");
            sync();
            reboot(RB_POWER_OFF);
            break;
    }
}
static char* opts[] = { "Snake" };

MenuModule gamesMenu = {
    .title = "Games",
    .options = opts,
    .options_count = 1,
    .on_select = handle
};
