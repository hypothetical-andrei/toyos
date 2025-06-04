#include "fs.h"

#define MAX_FILES 16
#define MAX_NAME_LEN 32
#define MAX_OPEN_FILES 8

struct file_entry {
    char name[MAX_NAME_LEN];
    void* data;
    size_t size;
};

struct open_file {
    struct file_entry* file;
    size_t offset;
    int in_use;
};

static struct file_entry file_table[MAX_FILES];
static struct open_file open_files[MAX_OPEN_FILES];

int file_create(const char* name, size_t size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].data == NULL) {
            void* mem = alloc();
            if (!mem) {
                uart_puts("alloc failed in file_create\n");
                return -1;
            }
            safe_strcpy(file_table[i].name, name, MAX_NAME_LEN);
            file_table[i].data = mem;
            file_table[i].size = size;
            return 0;
        }
    }
    return -1;
}

void file_seek(int fd, size_t offset) {
    if (fd >= 0 && fd < MAX_OPEN_FILES && open_files[fd].in_use) {
        open_files[fd].offset = offset;
    }
}

int file_open(const char* name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].data && safe_strncmp(file_table[i].name, name, MAX_NAME_LEN) == 0) {
            for (int j = 0; j < MAX_OPEN_FILES; j++) {
                if (!open_files[j].in_use) {
                    open_files[j].file = &file_table[i];
                    open_files[j].offset = 0;
                    open_files[j].in_use = 1;
                    return j;
                }
            }
        }
    }
    return -1;
}

size_t file_read(int fd, void* buf, size_t n) {
    if (fd < 0 || fd >= MAX_OPEN_FILES || !open_files[fd].in_use)
        return 0;

    struct open_file* of = &open_files[fd];
    size_t remaining = of->file->size - of->offset;
    size_t to_read = (n < remaining) ? n : remaining;

    safe_memcpy(buf, (uint8_t*)of->file->data + of->offset, to_read);
    of->offset += to_read;
    return to_read;
}

size_t file_write(int fd, const void* buf, size_t n) {
    if (fd < 0 || fd >= MAX_OPEN_FILES || !open_files[fd].in_use)
        return 0;

    struct open_file* of = &open_files[fd];
    size_t remaining = of->file->size - of->offset;
    size_t to_write = (n < remaining) ? n : remaining;

    safe_memcpy((uint8_t*)of->file->data + of->offset, buf, to_write);
    of->offset += to_write;
    return to_write;
}

void file_close(int fd) {
    if (fd >= 0 && fd < MAX_OPEN_FILES) {
        open_files[fd].in_use = 0;
    }
}

void file_list() {
    uart_puts("Files:\n");

    for (int i = 0; i < MAX_FILES; i++) {
        if (file_table[i].data != NULL) {
            uart_puts("  - ");
            uart_puts(file_table[i].name);
            uart_puts(" (size ");
            uart_putint((int)file_table[i].size);
            uart_puts(")\n");
        }
    }
}

int file_delete(const char* name) {
		uart_puts("File to delete: ");
		uart_puts(name);
		uart_puts("\n");
    for (int i = 0; i < MAX_FILES; i++) {
    		if (file_table[i].name[0] != '\0' && safe_strncmp(file_table[i].name, name, MAX_NAME_LEN) == 0) {
	      		uart_puts("Deleting file: ");
		        uart_puts(name);
		        uart_putc('\n');

            free_page(file_table[i].data);
            file_table[i].data = NULL;

            file_table[i].name[0] = '\0';

            file_table[i].size = 0;
            return 0; // success
        }
    }
    return -1; // not found
}

size_t file_get_size(int fd) {
    if (fd < 0 || fd >= MAX_OPEN_FILES || !open_files[fd].in_use)
        return 0;
    return open_files[fd].file->size;
}
