//#include <stdio.h>
#include <sys/reboot.h>
#include <unistd.h>
#include "../menu.h"

static void handle(int index, MenuModule* me)
{
    switch (index)
    {
        case 0:

            break;
        case 1:

            break;
    }
}
static char* opts[] = { "Snake" };

MenuModule gamesMenu = {
    .title = "Games",
    .options = opts,
    .options_count = 1,
    .running = 1,
    .on_select = handle
};
