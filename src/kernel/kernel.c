#include "kernel.h"
#include "../common/utility.h"
#include "../common/terminal.h"

void logStart(char* msg, int state)
{
    print(" ");
    if (state) { printc(msg, LIGHT_GREEN); } else { printc(msg, LIGHT_RED); }
    for (int i = 0; i < 50 - len(msg); i++) { printc(".", DARK_GRAY); }
    print(" [ ");
    if (state) { printc(" OK ", LIGHT_GREEN); } else { printc("ERR!", LIGHT_RED); }
    print(" ] \n");
}

void kmain() {
    clear();
    logStart("Kernel Memory", 1);
    logStart("Keyboard Driver", 1);
    logStart("VGA Text Mode", 1);
    logStart("File System", 0);
    logStart("Applications (1/3):", 1);
    logStart("- Terminal", 1);
    logStart("- File Manager", 0);
    logStart("- Calculator", 0);

    print("\n "); printcb("[ SYSTEM LOADED ]\n\n", WHITE, GREEN);

    while(1);
}
