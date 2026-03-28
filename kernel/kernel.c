#include "../common/utility.h"
#include "kernel.h"

void kmain() {
    clear();
    print("Jakas wiadomosc z kernela\n");
    printInt(2137);
    hideCursor();

    while(1);
}
