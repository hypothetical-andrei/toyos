#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdint.h>
#include <stddef.h>

void allocator_init();
void* alloc();
void free_page(void* ptr);
void* safe_memcpy(void* dest, const void* src, size_t n);

#endif
