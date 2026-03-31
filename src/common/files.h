#ifndef FILES_H
#define FILES_H

#define MAX_ENTRIES 256
#define NAME_LEN 32

typedef struct FileNode {
    char name[NAME_LEN];
    int isDirectory;
    char* content;
    int size;
    struct FileNode* children[MAX_ENTRIES];
    int entryCount;
} FileNode;

#endif
