#include "files.h"
#include "terminal.h"
#include "../common/utility.h"

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
void listFiles() {
    uint8_t buf[512];
    read_sector(QUNEFS_TABLE_SECTOR, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1) { // Plik
            print("- "); print(entries[i].name); print("\n");
        }
        else if (entries[i].type == 2) { // Folder
            print("[DIR] "); print(entries[i].name); print("/\n");
        }
    }
}
void touch(char* name) {
    uint8_t buf[512];
    read_sector(QUNEFS_TABLE_SECTOR, buf);
    QuneEntry* entries = (QuneEntry*)buf;
    for (int i = 0; i < 512 / sizeof(QuneEntry); i++) {
        if (entries[i].type == 0) {
            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            for(int j=0; j<31 && name[j]; j++) entries[i].name[j] = name[j];
            entries[i].type = 1;
            entries[i].start_lba = QUNEFS_DATA_START + i;
            write_sector(QUNEFS_TABLE_SECTOR, buf);
            print("Utworzono plik.\n");
            return;
        }
    }
}
void editFile(char* name, char* text) {
    uint8_t tableBuf[512];
    read_sector(QUNEFS_TABLE_SECTOR, tableBuf);
    QuneEntry* entries = (QuneEntry*)tableBuf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            uint8_t dataBuf[512];
            for(int j = 0; j < 512; j++) dataBuf[j] = 0;

            int writePos = 0;
            for(int readPos = 0; text[readPos] != '\0' && writePos < 511; readPos++) {
                // Jeśli znajdziemy \n w tekście
                if (text[readPos] == '\\' && text[readPos+1] == 'n') {
                    dataBuf[writePos++] = 10; // Dodaj bajt Nowej Linii (ASCII 10)
                    readPos++; // Pomiń literę 'n'
                } else {
                    dataBuf[writePos++] = text[readPos];
                }
            }

            write_sector(entries[i].start_lba, dataBuf);
            print("Zapisano z obsluga linii.\n");
            return;
        }
    }
    print("Nie znaleziono pliku.\n");
}
void catFile(char* name) {
    uint8_t tableBuf[512];
    read_sector(QUNEFS_TABLE_SECTOR, tableBuf);
    QuneEntry* entries = (QuneEntry*)tableBuf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            uint8_t dataBuf[512];

            read_sector(entries[i].start_lba, dataBuf);

            print(name); print(" contents:\n");
            printc((char*)dataBuf, LIGHT_CYAN);
            print("\n");
            return;
        }
    }

    printc("Blad: Nie znaleziono pliku: ", LIGHT_RED);
    print(name);
    print("\n");
}
void removeFile(char* name) {
    uint8_t buf[512];
    read_sector(QUNEFS_TABLE_SECTOR, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            entries[i].type = 0; // Oznaczamy jako wolny
            // Opcjonalnie zerujemy nazwę, żeby w hex-podglądzie było czysto
            for(int j=0; j<32; j++) entries[i].name[j] = 0;

            write_sector(QUNEFS_TABLE_SECTOR, buf);
            print("Plik usuniety.\n");
            return;
        }
    }
    print("Nie znaleziono pliku do usuniecia.\n");
}
void makeDir(char* name) {
    uint8_t buf[512];
    read_sector(QUNEFS_TABLE_SECTOR, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 0) { // Znaleźliśmy wolne miejsce w tabeli
            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            for(int j=0; j<31 && name[j]; j++) entries[i].name[j] = name[j];

            entries[i].type = 2;       // TYP 2 = FOLDER
            entries[i].start_lba = 0;  // Folder na razie nie potrzebuje sektora danych
            entries[i].size = 0;

            write_sector(QUNEFS_TABLE_SECTOR, buf);
            print("Folder utworzony.\n");
            return;
        }
    }
    print("Blad: Brak miejsca w tabeli.\n");
}
