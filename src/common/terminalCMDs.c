#include "utility.h"
#include "terminal.h"
#include "terminalCMDs.h"

extern char actualPath[512];

void help_qc(char* args)
{
    print("- help\n");
    print("- echo <message>\n");
    print("- clear\n");
    print("- \n");
    print("- \n");
    print("- \n");
    print("- \n");
}
void cd_qc(char* args) {
    if (is(args, "..")) {
        if (is(actualPath, "*")) { return; }
        cutEnd(actualPath, len(actualPath) - fndLast(actualPath, '/'));
        return;
    }
    copyStr(actualPath, addStr(addStr(actualPath, "/"), args));

}
void echo_qc(char* args) { printq("%s\n", args); }
void clear_qc(char* args) { clear(); }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }


TerminalCMD cmds[] = {
    {"help", help_qc},
    {"cd", cd_qc},
    {"echo", echo_qc},
    {"clear", clear_qc},
    // {"", },
    // {"", },
};
int cmd_count = sizeof(cmds) / sizeof(TerminalCMD);
