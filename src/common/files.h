#ifndef FILES_H
#define FILES_H

#include "../kernel/kernel.h"

#define QUNEFS_DATA_START 20

#pragma pack(push, 1)
typedef struct {
    char name[32];
    uint8_t type;
    uint32_t start_lba;
    uint32_t size;
} QuneEntry;
#pragma pack(pop)

void read_sector(uint32_t lba, uint8_t* buffer);
void write_sector(uint32_t lba, uint8_t* buffer);
int exists(char* name);
char* readFile(char* name);
void listFiles();
void touch(char* name);
void editFile(char* name, char* text);
void catFile(char* name);
void removeFile(char* name);
void removeDir(char* name);
void makeDir(char* name);
void changeDir(char* name);

#endif
