#include "allocator.h"

#define PAGE_SIZE         4096
#define NUM_PAGES         1024
#define HEAP_START        0x41000000
#define ALLOC_TABLE_SIZE  (NUM_PAGES * sizeof(uint8_t))

static uint8_t* alloc_table = (uint8_t*)HEAP_START;
static uint8_t* page_base   = (uint8_t*)(HEAP_START + ALLOC_TABLE_SIZE);

void allocator_init() {
    for (size_t i = 0; i < NUM_PAGES; i++) {
        alloc_table[i] = 0;  // mark all as free
    }
}

void* alloc() {
    for (size_t i = 0; i < NUM_PAGES; i++) {
        if (alloc_table[i] == 0) {
            alloc_table[i] = 1;
            return (void*)(page_base + i * PAGE_SIZE);
        }
    }
    return NULL;  // no free pages
}

void free_page(void* ptr) {
    if ((uint8_t*)ptr < page_base || (uint8_t*)ptr >= page_base + PAGE_SIZE * NUM_PAGES)
        return; // invalid pointer

    size_t index = ((uint8_t*)ptr - page_base) / PAGE_SIZE;
    alloc_table[index] = 0;
}

void* safe_memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = dest;
    const uint8_t* s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}
