#include <stdio.h>
#include <sys/reboot.h>
#include <unistd.h>
#include "menu.h"
#include "files.h"

int main()
{
    init_system_folders();
    run_menu_module(&mainMenu);
    printf("Exiting...\n");
    sync();
    reboot(RB_POWER_OFF);
    return 0;
}
