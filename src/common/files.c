#include "files.h"
#include "terminal.h"
#include "../common/utility.h"

extern char actualPath[STR_LEN * 4];
uint32_t current_dir_lba = 12; // Domyślnie ROOT to sektor 12

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
    read_sector(current_dir_lba, buf);
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
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;
    for (int i = 0; i < 512 / sizeof(QuneEntry); i++) {
        if (entries[i].type == 0) {
            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            for(int j=0; j<31 && name[j]; j++) entries[i].name[j] = name[j];
            entries[i].type = 1;
            entries[i].start_lba = QUNEFS_DATA_START + i;
            write_sector(current_dir_lba, buf);
            print("Utworzono plik.\n");
            return;
        }
    }
}
void editFile(char* name, char* text) {
    uint8_t tableBuf[512];
    read_sector(current_dir_lba, tableBuf);
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
    read_sector(current_dir_lba, tableBuf);
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
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            entries[i].type = 0; // Oznaczamy jako wolny
            // Opcjonalnie zerujemy nazwę, żeby w hex-podglądzie było czysto
            for(int j=0; j<32; j++) entries[i].name[j] = 0;

            write_sector(current_dir_lba, buf);
            print("Plik usuniety.\n");
            return;
        }
    }
    print("Nie znaleziono pliku do usuniecia.\n");
}
void removeDir(char* name) {
    uint8_t buf[512];
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 2 && is(entries[i].name, name)) {
            entries[i].type = 0;
            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            write_sector(current_dir_lba, buf);
            print("Folder usuniety.\n");
            return;
        }
    }
    print("Nie znaleziono folderu o tej nazwie.\n");
}
void makeDir(char* name) {
    uint8_t buf[512];
    read_sector(current_dir_lba, buf); // Czytamy AKTUALNY katalog
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 0) {
            // Znajdujemy wolny sektor dla nowego folderu
            // Na razie prymitywnie: 20 + i (powinieneś mieć licznik wolnych sektorów!)
            uint32_t new_folder_lba = 30 + i;

            // Czyścimy ten nowy sektor na dysku, żeby był pusty
            uint8_t empty[512];
            for(int j=0; j<512; j++) empty[j] = 0;
            write_sector(new_folder_lba, empty);

            // Zapisujemy wpis w obecnym katalogu
            for(int j=0; j<32; j++) entries[i].name[j] = 0;
            for(int j=0; j<31 && name[j]; j++) entries[i].name[j] = name[j];
            entries[i].type = 2;
            entries[i].start_lba = new_folder_lba;

            write_sector(current_dir_lba, buf);
            print("Utworzono podkatalog.\n");
            return;
        }
    }
}
void changeDir(char* name) {
    if (is(name, "*")) {
        current_dir_lba = 12;
        print("Wrocono do root.\n");
        actualPath[0] = '*';
        actualPath[1] = '\0';
        return;
    }

    uint8_t buf[512];
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 2 && is(entries[i].name, name)) {
            current_dir_lba = entries[i].start_lba;
            print("Zmieniono katalog na: "); print(name); print("\n");
            copyStr(actualPath, addStr(actualPath, "/"));
            copyStr(actualPath, addStr(actualPath, name));
            return;
        }
    }
    print("Nie znaleziono katalogu.\n");
}
