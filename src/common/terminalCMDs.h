#ifndef TERMINAL_CMD_H
#define TERMINAL_CMD_H

typedef struct {
    char* cmd;
    void (*handler)(char*);
} TerminalCMD;

#endif
