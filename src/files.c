#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "files.h"
#include "utils.h"

void list_files(const char* path) {
    struct dirent *de;
    DIR *dr = opendir(path);
    if (dr == NULL) {
        print_color("Błąd otwarcia katalogu!\n", RED);
        return;
    }
    printf("\n--- Zawartość %s ---\n", path);
    while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
        printf("  [%s] %s\n", (de->d_type == DT_DIR) ? "DIR" : "FILE", de->d_name);
    }
    closedir(dr);
}

void create_file(const char* name) {
    char path[128];
    snprintf(path, sizeof(path), "/mnt/%s", name);
    int fd = creat(path, 0666);
    if (fd != -1) {
        printf("Wpisz tekst do pliku: ");
        char buf[128];
        fgets(buf, sizeof(buf), stdin);
        write(fd, buf, strlen(buf));
        close(fd);
        print_color("Plik zapisany!\n", GREEN);
    } else perror("Błąd");
}

void create_dir(const char* name) {
    char path[128];
    snprintf(path, sizeof(path), "/mnt/%s", name);
    if (mkdir(path, 0777) == 0) print_color("Folder utworzony!\n", GREEN);
    else perror("Błąd");
}

void read_file(const char* name) {
    char path[128], ch;
    snprintf(path, sizeof(path), "/mnt/%s", name);
    FILE *f = fopen(path, "r");
    if (f) {
        printf("\n--- Treść %s ---\n", name);
        while ((ch = fgetc(f)) != EOF) putchar(ch);
        fclose(f);
        printf("\n");
    } else perror("Błąd");
}
