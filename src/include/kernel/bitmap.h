#ifndef OSDEV_BITMAP_H
#define OSDEV_BITMAP_H

#include "stdint-gcc.h"
#include "stddef.h"
#include "limine.h"
#include "font8x8.h"

void draw_pixel(struct limine_framebuffer *fb, size_t x, size_t y, uint32_t color);

void draw_char(struct limine_framebuffer *fb, char c, size_t x, size_t y, uint32_t color);

void draw_string(struct limine_framebuffer *fb, const char *str, size_t x, size_t y, uint32_t color);

#endif //OSDEV_BITMAP_H
