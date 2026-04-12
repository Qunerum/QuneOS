#include "files.h"
#include "terminal.h"
#include "../common/utility.h"
#include "../kernel/memory.h"

extern char* actualPath;
uint32_t current_dir_lba = 12;

void ide_wait_ready() { while (inb(0x1F7) & 0x80); }

void read_sector(uint32_t lba, uint8_t* buffer) {
    ide_wait_ready();
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, 1);
    outb(0x1F3, (uint8_t)lba);
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F7, 0x20);
    while (!(inb(0x1F7) & 0x08));
    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(0x1F0);
        buffer[i*2] = (uint8_t)data;
        buffer[i*2+1] = (uint8_t)(data >> 8);
    }
}

void write_sector(uint32_t lba, uint8_t* buffer) {
    ide_wait_ready();
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, 1);
    outb(0x1F3, (uint8_t)lba);
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F7, 0x30);
    while (!(inb(0x1F7) & 0x08));
    for (int i = 0; i < 256; i++) {
        uint16_t data = buffer[i*2] | (buffer[i*2+1] << 8);
        outw(0x1F0, data);
    }
}
int exists(char* name) {
    uint8_t* buf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;
    for (int i = 0; i < 16; i++) { if (entries[i].type != 0 && is(entries[i].name, name)) return 1; }
    return 0;
}

int chk(char* n) { if (exists(n)) { printc("Error: Name already exists!\n", LIGHT_RED); return 1; } return 0; }

char* readFile(char* name) {
    uint8_t* tableBuf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, tableBuf);
    QuneEntry* entries = (QuneEntry*)tableBuf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            char* data = (char*)kmalloc(512);
            read_sector(entries[i].start_lba, (uint8_t*)data);
            return data;
        }
    }
    return "";
}

void listFiles() {
    uint8_t* buf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;
    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1) { print("- "); print(entries[i].name); print("\n"); }
        else if (entries[i].type == 2) { print("[DIR] "); print(entries[i].name); print("/\n"); }
    }
}

void touch(char* name) {
    if (chk(name)) return;
    uint8_t* buf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;
    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 0) {
            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            for(int j=0; j<31 && name[j]; j++) entries[i].name[j] = name[j];
            entries[i].type = 1;
            entries[i].start_lba = QUNEFS_DATA_START + (current_dir_lba * 16) + i;
            write_sector(current_dir_lba, buf);
            print("Plik utworzony.\n");
            return;
        }
    }
}

void editFile(char* name, char* text) {
    uint8_t* tableBuf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, tableBuf);
    QuneEntry* entries = (QuneEntry*)tableBuf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            uint8_t* dataBuf = (uint8_t*)kmalloc(512);
            for (int j = 0; j < 511 && text[j]; j++) dataBuf[j] = text[j];
            write_sector(entries[i].start_lba, dataBuf);
            print("Zapisano.\n");
            return;
        }
    }
}

void catFile(char* name) {
    uint8_t* tableBuf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, tableBuf);
    QuneEntry* entries = (QuneEntry*)tableBuf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            uint8_t* dataBuf = (uint8_t*)kmalloc(512);
            read_sector(entries[i].start_lba, dataBuf);
            printc((char*)dataBuf, LIGHT_CYAN);
            print("\n");
            return;
        }
    }
}

void removeFile(char* name) {
    uint8_t* buf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            entries[i].type = 0;
            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            write_sector(current_dir_lba, buf);
            print("Usunieto plik.\n");
            return;
        }
    }
}

void removeDir(char* name) {
    uint8_t* buf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 2 && is(entries[i].name, name)) {
            entries[i].type = 0;
            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            write_sector(current_dir_lba, buf);
            print("Usunieto folder.\n");
            return;
        }
    }
}

void makeDir(char* name) {
    if (chk(name)) return;
    uint8_t* buf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 0) {
            // Bezpieczniejsza alokacja LBA dla podkatalogu
            uint32_t new_folder_lba = QUNEFS_DATA_START + (current_dir_lba * 2) + i;

            uint8_t* empty = (uint8_t*)kmalloc(512);
            write_sector(new_folder_lba, empty);

            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            for(int j=0; j<31 && name[j]; j++) entries[i].name[j] = name[j];
            entries[i].type = 2;
            entries[i].start_lba = new_folder_lba;

            write_sector(current_dir_lba, buf);
            print("Folder utworzony.\n");
            return;
        }
    }
}

void changeDir(char* name) {
    if (is(name, "*")) {
        current_dir_lba = 12;
        copyStr(actualPath, "*");
        print("Root.\n");
        return;
    }

    uint8_t* buf = (uint8_t*)kmalloc(512);
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 2 && is(entries[i].name, name)) {
            current_dir_lba = entries[i].start_lba;
            copyStr(actualPath, addStr(addStr(actualPath, "/"), name));
            return;
        }
    }
}
