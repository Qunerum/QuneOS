#include "../common/utility.h"
#include "kernel.h"

void kmain() {
    clear();
    hideCursor();
    for (int i = 0; i < 50; i++) { printInt(i); print(".\n"); delay(1000); }

    while(1);
}
