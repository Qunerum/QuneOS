#ifndef FILES_H
#define FILES_H

#define QOS_ROOT "/qos"
#define QOS_USER_ROOT QOS_ROOT "/user"
#define QOS_CONFIG QOS_ROOT "/system/config"

void get_file_info(const char* filename, char* out_type, long* out_size);
char** get_file_list(const char* path);
void create_file(const char* name);
void create_dir(const char* name);

#endif
