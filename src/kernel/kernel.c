#include "kernel.h"
#include "../common/utility.h"
#include "../common/terminal.h"
#include "../common/terminalCMDs.h"

static char actualPath[512] = "*";

void logStart(char* msg, int state)
{
    print(" ");
    if (state) { printc(msg, LIGHT_GREEN); } else { printc(msg, LIGHT_RED); }
    for (int i = 0; i < 50 - len(msg); i++) { printc(".", DARK_GRAY); }
    print(" [ ");
    if (state) { printc(" OK ", LIGHT_GREEN); } else { printc("ERR!", LIGHT_RED); }
    print(" ] \n");
}
void runCmd(char* input)
{
    for (int i = 0; i < cmd_count; i++) { if (startsWith(input, cmds[i].cmd)) { cmds[i].handler(cut(input, len(cmds[i].cmd) + 1)); return; } }
    print("Unkown command!\n");
}

void kmain() {
    clear();
    logStart("Kernel Memory", 1);
    logStart("Keyboard Driver", 1);
    logStart("VGA Text Mode", 1);
    logStart("File System", 0);
    logStart("Applications (1/1):", 1);
    logStart("- Terminal", 1);

    print("\n /--------------------------------\\\n ");
    print("|"); printb("  ", BLACK); printb("  ", BLUE); printb("  ", GREEN); printb("  ", CYAN);
    printb("  ", RED); printb("  ", MAGENTA); printb("  ", BROWN); printb("  ", LIGHT_GRAY);
    printb("  ", DARK_GRAY); printb("  ", LIGHT_BLUE); printb("  ", LIGHT_GREEN); printb("  ", LIGHT_CYAN);
    printb("  ", LIGHT_RED); printb("  ", LIGHT_MAGENTA); printb("  ", YELLOW); printb("  ", WHITE); print("|\n ");
    print("\\--------------------------------/\n\n ");
    printcb("[ SYSTEM LOADED ]\n\n", WHITE, GREEN);

    char cmd[CMD_MAX];
    while(1)
    {
        print(" '"); printc(actualPath, WHITE); print("': ");
        readLine(cmd, CMD_MAX);
        if (len(cmd) > 0) { add_to_history(cmd); }
        runCmd(cmd);
    }
}
