#ifndef TERMINAL_CMDS_H
#define TERMINAL_CMDS_H

typedef struct {
    char* cmd;
    void (*handler)(char*);
} qosCMD;

extern qosCMD cmds[];
extern int cmd_count;
void runCmd(char* cmd, char* args);

#endif
