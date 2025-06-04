#include "exec.h"

int exec_file(const char* name) {
    int fd = file_open(name);
    if (fd < 0) {
        uart_puts("Failed to open file\n");
        return -1;
    }

    void* exec_addr = (void*)0x42000000;
    file_seek(fd, 0);  // rewind to start

    size_t read = file_read(fd, exec_addr, file_get_size(fd));  // or max size
    if (read == 0) {
        uart_puts("Failed to read file\n");
        return -1;
    }

    uart_puts("Executing file at 0x42000000...\n");

    // Cast and call
    void (*entry)() = (void (*)())exec_addr;
    entry();  // 🎉 execute the loaded code

    // NOTE: you won’t return unless the program ends with `ret`
    return 0;
}
