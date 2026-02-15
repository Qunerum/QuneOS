#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "files.h"
#include "utils.h"

void mkdir_p(const char *path) {
    char temp[512];
    char *p = NULL;
    size_t len;

    snprintf(temp, sizeof(temp), "%s", path);
    len = strlen(temp);

    if (temp[len - 1] == '/') temp[len - 1] = 0;

    for (p = temp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(temp, 0755);
            *p = '/';
        }
    }
    mkdir(temp, 0755);
}

void init_system_folders() {
    mkdir_p("./qos/system/config");
    mkdir_p("./qos/user/admin/documents");
    mkdir_p("./qos/user/admin/games");
}

void expand_path(const char* input_path, char* output_path) {
    if (input_path[0] == '~') { sprintf(output_path, ".%s/%s%s", QOS_USER_ROOT, "admin", input_path + 1); } else
        if (input_path[0] == '/') { sprintf(output_path, ".%s", input_path); } else { strcpy(output_path, input_path); }
}

char** get_file_list(const char* logical_path) {
    char physical_path[512]; // Bufor na "prawdziwą" ścieżkę
    expand_path(logical_path, physical_path); // Translacja

    struct dirent *de;
    DIR *dr = opendir(physical_path); // Otwieramy to, co wypluł expand_path

    if (dr == NULL) {
        return NULL;
    }

    // --- Reszta Twojej logiki bez zmian ---
    int count = 0;
    while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
        count++;
    }

    char** list = malloc((count + 1) * sizeof(char*));
    rewinddir(dr);

    int i = 0;
    while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
        list[i] = strdup(de->d_name);
        i++;
    }
    list[i] = NULL;

    closedir(dr);
    return list;
}

void create_file(const char* name) {
    char path[128];
    snprintf(path, sizeof(path), "./qos/%s", name);
    int fd = creat(path, 0666);
    if (fd != -1) {
        printf("Wpisz tekst do pliku: ");
        char buf[128];
        fgets(buf, sizeof(buf), stdin);
        write(fd, buf, strlen(buf));
        close(fd);
    } else perror("ERROR");
}

void create_dir(const char* name) {
    char path[128];
    snprintf(path, sizeof(path), "/mnt/%s", name);
    if (mkdir(path, 0777) == 0) print_color("Folder utworzony!\n", GREEN);
    else perror("Błąd");
}
