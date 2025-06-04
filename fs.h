#ifndef FS_H
#define FS_H

#include "allocator.h"
#include "safe_strings.h"
#include "console.h"
#include <stddef.h>

int file_create(const char* name, size_t size);
void file_close(int fd);
int file_open(const char* name);
size_t file_read(int fd, void* buf, size_t n);
void file_seek(int fd, size_t offset);
size_t file_write(int fd, const void* buf, size_t n);
void file_list();
int file_delete(const char* name);
size_t file_get_size(int fd);

#endif
