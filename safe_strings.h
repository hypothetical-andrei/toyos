#ifndef SAFE_STRINGS_H
#define SAFE_STRINGS_H

#include <stddef.h>

int safe_strncmp(const char* s1, const char* s2, size_t n);
void safe_strcpy(char* dest, const char* src, size_t max_len);
size_t safe_strlen(const char* s, size_t max_len);

#endif
