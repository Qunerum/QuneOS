#ifndef FILES_H
#define FILES_H

#define QOS_ROOT "/qos"
#define QOS_USER_ROOT QOS_ROOT "/user"
#define QOS_CONFIG QOS_ROOT "/system/config"

void mkdir_p(const char *path);
void init_system_folders();
void expand_path(const char* input_path, char* output_path);

char** get_file_list(const char* path);
void create_file(const char* name);
void create_dir(const char* name);

#endif
