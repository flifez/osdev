#include "bitmap.h"

static void draw_pixel(struct limine_framebuffer *fb, size_t x, size_t y, uint32_t color) {
    volatile uint32_t *fb_ptr = fb->address;
    fb_ptr[y * (fb->pitch / 4) + x] = color;
}

static void draw_char(struct limine_framebuffer *fb, char c, size_t x, size_t y, uint32_t color) {
    for (int row = 0; row < 8; row++) {
        uint8_t row_data = FONT_8X8[(unsigned char)c][row];
        for (int col = 0; col < 8; col++) {
            if (row_data & (1 << (7 - col))) {
                draw_pixel(fb, x + col, y + row, color);
            }
        }
    }
}

static void draw_string(struct limine_framebuffer *fb, const char *str, size_t x, size_t y, uint32_t color) {
    size_t curr_x = x;
    while (*str) {
        draw_char(fb, *str, curr_x, y, color);
        curr_x += 8;
        str++;
    }
}
