#include "drivers/screen.h"
#include "../lib/text.h"
#include "kernel/OSINFO.h"
#include "terminal.h"
#include "terminalCmds.h"
#include "OSINFO.h"

void help_qos(char* args) { for (int i = 0; i < cmd_count; i++) { print("- ", TEXT_COLOR); print(cmds[i].cmd, !(i % 2) ? ORANGE : YELLOW); printChar('\n', TEXT_COLOR); } }
void version_qos(char* args) { print("The current version of QuneOS is: ", DARK_GREEN); print(OS_VERSION "\n", GREEN); }

qosCMD cmds[] = {
    {"help", help_qos},
    {"version", version_qos},
};
int cmd_count = sizeof(cmds) / sizeof(qosCMD);

void runCmd(char* cmd, char* args) {
    for (int i = 0; i < cmd_count; i++) { if(is(cmds[i].cmd, cmd)) { cmds[i].handler(args); return; } }
    print("Cannot find command: '", RED);
    print(cmd, RED);
    print("'!\nUse 'help' to see all commands.\n", RED);
}
