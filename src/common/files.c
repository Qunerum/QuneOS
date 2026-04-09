#include "files.h"
#include "terminal.h"
#include "../common/utility.h"
#include "../kernel/memory.h"

extern char actualPath[STR_LEN * 4];
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
void listFiles() {
    uint8_t buf[512];
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1) {
            print("- "); print(entries[i].name); print("\n");
        }
        else if (entries[i].type == 2) {
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
            uint8_t* entry_ptr = (uint8_t*)&entries[i];
            for(int j=0; j<sizeof(QuneEntry); j++) entry_ptr[j] = 0;
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
            // Zamiast uint8_t dataBuf[512], używamy kmalloc
            uint8_t* dataBuf = (uint8_t*)kmalloc(512);

            // Kopiujemy tekst do naszego nowego bufora na stercie
            for (int j = 0; j < 512; j++) dataBuf[j] = 0; // Czyszczenie
            for (int j = 0; j < 511 && text[j]; j++) {
                dataBuf[j] = text[j];
            }

            // Zapisujemy na dysk
            write_sector(entries[i].start_lba, dataBuf);

            print("Zapisano zmiany w pliku: ");
            print(name);
            print("\n");
            return;
        }
    }
    printc("Blad: Nie mozna edytowac. Nie znaleziono pliku: ", LIGHT_RED);
    print(name);
    print("\n");
}
void catFile(char* name) {
    // Bufor na wpisy w katalogu (może zostać na stosie, bo jest mały i krótko używany)
    uint8_t tableBuf[512];
    read_sector(current_dir_lba, tableBuf);
    QuneEntry* entries = (QuneEntry*)tableBuf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 1 && is(entries[i].name, name)) {
            // DYNAMICZNA ALOKACJA: prosimy o 512 bajtów ze sterty
            uint8_t* dataBuf = (uint8_t*)kmalloc(512);

            // Czytamy dane do zaalokowanej pamięci
            read_sector(entries[i].start_lba, dataBuf);

            print(name); print(" contents:\n");
            printc((char*)dataBuf, LIGHT_CYAN);
            print("\n");

            // W naszym prostym kmalloc nie mamy free(),
            // więc pamięć zostaje zajęta do restartu,
            // ale przy 512 bajtach i megabajtach RAMu to nie problem.
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
            entries[i].type = 0;
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
    read_sector(current_dir_lba, buf);
    QuneEntry* entries = (QuneEntry*)buf;

    for (int i = 0; i < 16; i++) {
        if (entries[i].type == 0) {
            uint32_t new_folder_lba = 30 + i;

            uint8_t empty[512];
            for(int j=0; j<512; j++) empty[j] = 0;
            write_sector(new_folder_lba, empty);

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
