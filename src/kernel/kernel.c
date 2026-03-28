#include "../common/utility.h"
#include "kernel.h"

void kmain() {
    clear();
    print("Podaj swoje imie: ");

    char name[50];
    readLine(name, 50);

    print("Witaj, ");
    print(name);
    print("!\n");

    print("Twoj bufor jest pod adresem: ");
    printHex((uintptr_t)name);

    while(1);
}
