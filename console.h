#ifndef CONSOLE_H
#define CONSOLE_H

void uart_putc(char c);
void uart_puts(const char* str);
void uart_putint(int value);

#endif
