#include "files.h"
#include "utility.h"
#include "terminal.h"
#include "terminalCMDs.h"

extern char actualPath[STR_LEN * 4];

void help_qc(char* args) { for (int i = 0; i < cmd_count; i++) { printq("- %s\n", cmds[i]); } }
void cd_qc(char* args) {
    if (len(args) == 0) return;

    // 1. Obsługa "cd .." (wyjście w górę)
    if (is(args, "..")) {

            int lastSlash = fndLast(actualPath, '/');
            if (lastSlash != -1) {
                cutEnd(actualPath, len(actualPath) - lastSlash);
            }
        } else {
        printc("Blad: Katalog nie istnieje!\n", LIGHT_RED);
    }
}
void ls_qc(char* args) { listFiles(); }
void touch_qc(char* args) { touch(args); }
void mkDir_qc(char* args) { makeDir(args); }
void rm_qc(char* args) { removeFile(args); }
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
    {"mkdir", mkDir_qc},
    {"rm", rm_qc},
    {"edit", write_qc},
    {"cat", cat_qc},

    {"echo", echo_qc},
    {"clear", clear_qc}
};
int cmd_count = sizeof(cmds) / sizeof(TerminalCMD);
