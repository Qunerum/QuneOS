#include "files.h"
#include "utility.h"
#include "terminal.h"
#include "terminalCMDs.h"

extern char* actualPath;

int has_permission() {
    if (is(actualPath, "*")) {
        printc("Access Denied: Cannot modify system directory (*)\n", LIGHT_RED);
        return 0;
    }
    return 1;
}

void help_qc(char* args) { for (int i = 0; i < cmd_count; i++) { printq("- %s\n", cmds[i].cmd); } } // help

void look_qc(char* args) { listFiles(); } // look
void view_qc(char* args) { catFile(args); } // view file
void newFile_qc(char* args) { if (has_permission()) touch(args); } // create new file
void write_qc(char* args) {
     if (has_permission())
     {
        char a[STR_LEN];
        char b[STR_LEN];
        split(args, ' ', a, b);
        editFile(a, b);
     }
} // write file
void build_qc(char* args) { if (has_permission()) makeDir(args); } // create new dir
void cd_qc(char* args) { changeDir(args); } // change direction
void erase_qc(char* args) { if (has_permission()) removeFile(args); } // remove file
void destruct_qc(char* args) { if (has_permission()) removeDir(args); } // remove dir
void echo_qc(char* args) { printq("%s\n", args); } // print in terminal
void clear_qc(char* args) { clear(); } // clear terminal
void mmr_qc(char* args) {
    extern unsigned long get_heap_pointer();
    unsigned long current = get_heap_pointer();
    unsigned long base = 0x200000;

    printc("--- QuneOs Memory Report ---\n", LIGHT_MAGENTA);
    print("Heap Base: "); printHex(base);
    print("\nHeap Curr: "); printHex(current);
    print("\nAllocated: "); printInt(current - base); print(" bytes\n");
}
//void _qc(char* args) { }


TerminalCMD cmds[] = {
    {"help", help_qc},
    {"look", look_qc},
    {"view", view_qc},
    {"new", newFile_qc},
    {"write", write_qc},
    {"build", build_qc},
    {"cd", cd_qc},
    {"erase", erase_qc},
    {"destruct", destruct_qc},
    {"echo", echo_qc},
    {"clear", clear_qc},
    {"mmr", mmr_qc}
};
int cmd_count = sizeof(cmds) / sizeof(TerminalCMD);
