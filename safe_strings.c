#include "safe_strings.h"

int safe_strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        unsigned char c1 = (unsigned char)s1[i];
        unsigned char c2 = (unsigned char)s2[i];

        if (c1 != c2) {
            return c1 - c2;
        }

        if (c1 == '\0') {
            break;
        }
    }
    return 0;
}

void safe_strcpy(char* dest, const char* src, size_t max_len) {
    size_t i = 0;
    for (; i < max_len - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0'; // always null-terminate
}

size_t safe_strlen(const char* s, size_t max_len) {
    size_t i = 0;
    while (i < max_len && s[i] != '\0') {
        i++;
    }
    return i;
}
