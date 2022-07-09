#ifndef INITFS_H
#define INITFS_H

#include <stddef.h>
#include <stdint.h>

#define MAX_FILES_IN_INITRD 32
#define MAX_FILENAME_SIZE 128

struct file
{
    char name[128];
    uintptr_t begin;
    uintptr_t end;
    uintptr_t cursor;
};

void register_new_file(const char* name, uintptr_t begin, uintptr_t end);
struct file *get_files();
struct file *get_file(const char *name);

#endif