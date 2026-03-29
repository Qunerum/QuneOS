// #include "../kernel/kernel.h"
#include "../common/terminal.h"
#include "terminalCMDs.h"

void echo_qc(char* args) { printq("%s\n", args); }
void clear_qc(char* args) { clear(); }
// void _qc(char* args) { }
// void _qc(char* args) { }
// void _qc(char* args) { }
// void _qc(char* args) { }
// void _qc(char* args) { }
// void _qc(char* args) { }
// void _qc(char* args) { }
// void _qc(char* args) { }
// void _qc(char* args) { }


TerminalCMD cmds[] = {
    {"echo", echo_qc},
    {"clear", clear_qc},
    // {"", },
    // {"", },
};
int cmd_count = sizeof(cmds) / sizeof(TerminalCMD);
