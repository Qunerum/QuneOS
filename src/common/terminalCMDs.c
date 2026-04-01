#include "files.h"
#include "utility.h"
#include "terminal.h"
#include "terminalCMDs.h"

extern char actualPath[STR_LEN * 4];

void help_qc(char* args) { for (int i = 0; i < cmd_count; i++) { printq("- %s\n", cmds[i]); } }
void cd_qc(char* args) { changeDir(args); }
void ls_qc(char* args) { listFiles(); }
void touch_qc(char* args) { touch(args); }
void rm_qc(char* args) { removeFile(args); }
void mkDir_qc(char* args) { makeDir(args); }
void rmDir_qc(char* args) { removeDir(args); }
void write_qc(char* args) {
    char a[STR_LEN];
    char b[STR_LEN];
    split(args, ' ', a, b);
    editFile(a, b);
}
void cat_qc(char* args) { catFile(args); }

void echo_qc(char* args) { printq("%s\n", args); }
void clear_qc(char* args) { clear(); }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }
//void _qc(char* args) { }


TerminalCMD cmds[] = {
    {"help", help_qc},
    {"cd", cd_qc},
    {"ls", ls_qc},
    {"touch", touch_qc},
    {"rm", rm_qc},
    {"mkdir", mkDir_qc},
    {"rmdir", rmDir_qc},
    {"edit", write_qc},
    {"cat", cat_qc},

    {"echo", echo_qc},
    {"clear", clear_qc}
};
int cmd_count = sizeof(cmds) / sizeof(TerminalCMD);
