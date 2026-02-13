#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>

#include "utils.h"
#include "files.h"

int main() {
    // Przygotowanie systemów
    mkdir("/proc", 0755); mount("proc", "/proc", "proc", 0, NULL);
    mkdir("/sys", 0755);  mount("sysfs", "/sys", "sysfs", 0, NULL);
    mkdir("/dev", 0755);  mknod("/dev/sda", S_IFBLK | 0660, makedev(8, 0));
    mkdir("/mnt", 0777);

    clear_screen();
    print_color("#######################################\n", CYAN);
    print_color("#          WITAJ W QuneOs             #\n", CYAN);
    print_color("#######################################\n", CYAN);

    if (mount("/dev/sda", "/mnt", "ext4", 0, NULL) == 0) {
        print_color("[OK] Dysk zamontowany w /mnt\n", GREEN);
    } else {
        print_color("[FAIL] Brak dysku twardego!\n", RED);
    }

    char choice, name[64];
    while(1) {
        printf("\n1. Lista  2. Plik  3. Folder  4. Czytaj  5. RAM  q. Poweroff\nWybor: ");
        choice = getchar(); while(getchar() != '\n');

        switch(choice) {
            case '1': list_files("/mnt"); break;
            case '2': printf("Nazwa pliku: "); scanf("%s", name); while(getchar() != '\n'); create_file(name); break;
            case '3': printf("Nazwa folderu: "); scanf("%s", name); while(getchar() != '\n'); create_dir(name); break;
            case '4': printf("Ktory plik: "); scanf("%s", name); while(getchar() != '\n'); read_file(name); break;
            case '5': system("head -n 3 /proc/meminfo"); break;
            case 'q': sync(); reboot(RB_POWER_OFF); break;
        }
    }
    return 0;
}
