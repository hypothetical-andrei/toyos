#define UART0_BASE 0x09000000

#define UARTDR     (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UARTFR     (*(volatile unsigned int *)(UART0_BASE + 0x18))

void uart_putc(char c) {
    while (UARTFR & (1 << 5)) ; // Wait until TX FIFO is not full
    UARTDR = c;
}

void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void uart_putint(int value) {
    char buf[12]; // enough for 32-bit signed int: "-2147483648\0"
    int i = 0;
    int is_negative = 0;

    if (value == 0) {
        uart_putc('0');
        return;
    }

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    // Extract digits in reverse order
    while (value > 0 && i < sizeof(buf) - 1) {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (is_negative) {
        buf[i++] = '-';
    }

    // Print in correct order
    while (i > 0) {
        uart_putc(buf[--i]);
    }
}
