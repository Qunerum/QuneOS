#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../lib/text.h"
// #include "kernel/memory.h"
#include "drivers/mouse.h"
#include "terminal.h"
#include "terminalCmds.h"

void printState(char* text, uint32_t result) {
    print(text, result == 0 ? CARMINE : TEXT_COLOR);
    for (int i = 0; i < 15 - len(text); i++) { printChar('.', DARK_GRAY); }
        print("[ ", DARK_GRAY);
    if (result == 0) {
        print("ERR!", RED);
        print(" ]\n", DARK_GRAY);
    } else {
        char hexBuf[12];
        intToHex(result, hexBuf);
        print(" OK ", GREEN);
        print(" ] at", DARK_GRAY);
        print(hexBuf, GRAY);
        printChar('\n', TEXT_COLOR);
    }
}
void checkColors() {
    printChar('\n', BLACK); // Normal
    printChar('\x80', GRAY);
    printChar('\x80', RED);
    printChar('\x80', GREEN);
    printChar('\x80', BLUE);
    printChar('\x80', MAGENTA);
    printChar('\x80', ORANGE);
    printChar('\n', BLACK); // Light
    printChar('\x80', WHITE);
    printChar('\x80', CARMINE);
    printChar('\x80', LIME);
    printChar('\x80',CYAN);
    printChar('\x80', PINK);
    printChar('\x80', YELLOW);
    printChar('\n', BLACK); // Dark
    printChar('\x80', DARK_GRAY);
    printChar('\x80', DARK_RED);
    printChar('\x80', DARK_GREEN);
    printChar('\x80', DARK_BLUE);
    printChar('\x80', PURPLE);
    printChar('\x80', BROWN);
    printChar('\n', BLACK);
}

extern char input[MAX_STRING_LEN];
extern volatile int mouseX;
extern volatile int mouseY;
extern volatile int mouseUpdated;
extern volatile int keyboardUpdated;
extern volatile char lastChar;

// __attribute__((section(".text.prologue")))
void _start_c() {
    struct vbe_mode_info* vbe = (struct vbe_mode_info*)0x8000;
    if (vbe->physbase == 0) {
        vbe->physbase = 0xFD000000;
        vbe->pitch = 4096;
        vbe->bpp = 32;
        vbe->width = 1024;
        vbe->height = 768;
    }
    printState("Screen", initScreen(vbe));
    printState("Terminal", initTerminal());
    printState("IDT", init_idt());
    printState("Keyboard", initKeyboard());
    printState("Mouse", initMouse());
    __asm__ volatile("sti");
    while ((inb(0x64) & 1) == 1) inb(0x60);

    checkColors();

    print("\nWelcome to QuneOS!\n", GREEN);

    draw_text(-400, 0, "\x80 , \x81 , \x82 , \x83 , \x84 , \x85 , \x86 , \x87 , \x88 , \x89 \n", 2, TEXT_COLOR);

    char tokenCmd[MAX_STRING_LEN];
    char tokenArg[MAX_STRING_LEN];

    while(1) {
        print("QuneOS \x84 ", TEXT_COLOR);
        setInput(1);
        inputEnable = 1;

        while(inputEnable) {
            // 1. Bezpieczna obsługa myszy poza przerwaniem
            if (mouseUpdated) {
                // Wyłączamy przerwania tylko na krytyczny moment odczytu i zapisu flagi
                __asm__ volatile("cli");
                mouseUpdated = 0;
                __asm__ volatile("sti");

                // Odświeżamy kursor na ekranie
                mouse_restore_background();
                mouse_draw();
            }

            // 2. Obsługa klawiatury (zostaje bez zmian)
            if (keyboardUpdated) {
                char c = lastChar;
                keyboardUpdated = 0;

                if (c == '\n') {
                    inputEnable = 0;
                    printChar('\n', TEXT_COLOR);
                }
                else if (c == '\t') {
                    print("    ", TEXT_COLOR);
                    addStr(input, "    ", 256);
                }
                else if (c == '\b') {
                    int l = len(input);
                    if (l > 0) {
                        input[l - 1] = '\0';
                        printChar('\b', BACKGROUND_COLOR);
                    }
                }
                else {
                    printChar(c, INPUT_COLOR);
                    addChar(input, c, 256);
                }
            }
        }

        // Wykonanie komendy po wyjściu z pętli (gdy inputEnable == 0)
        char* cmd = getInput();
        if (len(cmd) == 0) { continue; }
        else {
            if (contains(cmd, ' ')) {
                splitStart(cmd, ' ', tokenCmd, tokenArg, MAX_STRING_LEN);
                runCmd(tokenCmd, tokenArg);
            } else {
                runCmd(cmd, "");
            }
            mouse_draw();
        }
    }
}
