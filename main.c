#include "fs.h"
#include "safe_strings.h"
#include "allocator.h"
#include "console.h"
#include "exec.h"

// external file
extern uint8_t _binary_prog_bin_start[];
extern uint8_t _binary_prog_bin_end[];
extern uint8_t _binary_sample_bin_start[];
extern uint8_t _binary_sample_bin_end[];

void main() {
	allocator_init();
    uart_puts("Hello from QEMU virt board!\n");
    
    // file_create("test.txt", 4096);


    // int fd = file_open("test.txt");

    // const char* msg = "I am a message from the file!";
    // size_t written = file_write(fd, msg, safe_strlen(msg, 255));
    // if (written == 0) {
    // 	uart_puts("Nothing written to file");
    // }

    // char buf[32] = {0};

    // file_seek(fd, 0);
    // file_read(fd, buf, sizeof(buf));

    // uart_puts("Read from file:\n");
    // uart_puts(buf);
    // uart_puts("\n");

    // file_create("data.txt", 4096);

    // uart_puts("Before deletion:\n");
    // file_list();

    // file_delete("data.txt");

    // uart_puts("After deletion:\n");
    // file_list();

    // file_close(fd);
    
    file_create("prog.bin", 4096);

    int fd = file_open("prog.bin");
    if (fd >= 0) {
        size_t size = _binary_prog_bin_end - _binary_prog_bin_start;
        file_write(fd, _binary_prog_bin_start, size);
        file_close(fd);
    }


    uart_puts("prog.bin loaded!\n");

    exec_file("prog.bin");  // this should call the entry point

    // Any code after this might not run if the binary doesn't `ret`
    uart_puts("Back in kernel\n");
    file_close(fd);
   
    // file_create("sample.bin", 4096);

    // int fd = file_open("sample.bin");
    // if (fd >= 0) {
    //     size_t size = _binary_sample_bin_end - _binary_sample_bin_start;
    //     file_write(fd, _binary_sample_bin_start, size);
    //     file_close(fd);
    // }


    // uart_puts("sample.bin loaded!\n");

    // exec_file("sample.bin");  // this should call the entry point

    // // Any code after this might not run if the binary doesn't `ret`
    // uart_puts("Back in kernel\n");
    // file_close(fd);
}
