#include <stdio.h>
#include <sys/reboot.h>
#include <unistd.h>
#include "menu.h"

int main()
{
    run_menu_module(&mainMenu);
    printf("Exiting...\n");
    sync();
    reboot(RB_POWER_OFF);
    return 0;
}
