#include <stdio.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../menu.h"
#include "../files.h"
#include "../utils.h"

static char** current_files = NULL;
static int current_files_count = 0;

void format_size(long bytes, char* out) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int i = 0;
    double size = bytes;

    while (size >= 1024 && i < 3) {
        size /= 1024;
        i++;
    }
    if (i == 0) sprintf(out, "%ld B", bytes);
    else sprintf(out, "%.1f %s", size, units[i]);
}
void get_display_info(const char* full_name, char* out_name, char* out_type) {
    struct stat st;
    if (stat(full_name, &st) != 0) return;

    if (S_ISDIR(st.st_mode)) {
        strcpy(out_name, full_name);
        strcpy(out_type, "DIR");
        return;
    }
    char* dot = strrchr(full_name, '.');

    if (!dot || dot == full_name) {
        strcpy(out_name, full_name);
        strcpy(out_type, "FILE");
    } else {
        int name_len = dot - full_name;
        strncpy(out_name, full_name, name_len);
        out_name[name_len] = '\0';
        strcpy(out_type, dot + 1);
    }
}
void clear_current_files() {
    if (current_files == NULL) return;
    for (int i = 0; i < current_files_count; i++) free(current_files[i]);
    free(current_files);
    current_files = NULL;
}
static char currentPath[512] = "/";
void refresh_explorer(MenuModule* me) {
    clear_current_files();
    current_files = get_file_list(currentPath);
    current_files_count = 1;
    if (current_files) { while(current_files[current_files_count - 1] != NULL) current_files_count++; }
    me->options_count = current_files_count;
}
void go_back_dir(char* path) {
    if (path == NULL || strcmp(path, "/") == 0 || strlen(path) == 0) return;
    char* last_slash = strrchr(path, '/');
    if (last_slash != NULL) { if (last_slash == path) strcpy(path, "/"); else *last_slash = '\0'; }
}
static char* opts[] = { "Write", "Remove", "Rename", "Return" };
static void render(MenuModule* me, int c, int index) {
    if (current_files == NULL) refresh_explorer(me);

    printf("+--- Files -----------+\n");
    char ou[512];
    expand_path(currentPath, ou);
    printf("+--- Current path: %-8s ---+ %d\n", ou, index);
    printf("+--- Name ------------+-- type --+-- size --+\n");
    if (index == 0) printf(BG_BLUE WHITE);
    printf("| Return [..]         |          |          |\n" RESET);
    for (int i = 1; i < current_files_count; i++) {
        char displayName[256] = {0};
        char type[10] = {0};
        char sizeStr[15] = {0};
        struct stat st;

        stat(current_files[i - 1], &st);
        get_display_info(current_files[i - 1], displayName, type);
        format_size(st.st_size, sizeStr);

        if (i == index) printf(BG_BLUE WHITE);
        printf("| %-19.19s | %-8.8s | %8s | %d\n" RESET, displayName, type, sizeStr, i);
    }
    printf("+---------------------+----------+----------+\n");
}
static void handle(int index)
{
    if (index == 0)
    {
        go_back_dir(currentPath);
        clear_current_files();
        return;
    }
    if (current_files[index - 1] != NULL)
    {
        strcat(currentPath, current_files[index - 1]);
        clear_current_files();
        return;
    }
}

MenuModule fileApp = {
    .title = "Files",
    .options = opts,
    .options_count = 1,
    .on_select = handle,
    .custom_render = render
};
