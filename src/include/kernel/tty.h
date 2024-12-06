#ifndef OSDEV_TTY_H
#define OSDEV_TTY_H

#include <stddef.h>
#include <stdint-gcc.h>

void t_initialize(void);
void t_setcolor(uint32_t color);
void t_putentryat(unsigned char c, uint32_t color, size_t x, size_t y);
void t_scroll(int line);
void t_delete_last_line();
void t_putchar(char c);
void t_write(const char *data, size_t size);
void t_wstring(const char *data);

#endif //OSDEV_TTY_H
