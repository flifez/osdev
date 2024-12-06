#include "include/kernel/tty.h"
#include "include/kernel/bitmap.h"
#include "include/framebuffer.h"

static size_t t_row;
static size_t t_col;
static uint32_t t_color;
static struct limine_framebuffer *t_fb;

void t_initialize(void) {
    t_row = 0;
    t_col = 0;
    t_color = 0x00FFFFFF;

    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count < 1) {
        return;
    }
    
    t_fb = framebuffer_request.response->framebuffers[0];
    
    for (size_t y = 0; y < t_fb->height; y++) {
        for (size_t x = 0; x < t_fb->width; x++) {
            draw_pixel(t_fb, x, y, 0);
        }
    }
}

void t_setcolor(uint32_t color) {
    t_color = color;
}

void t_putentryat(unsigned char c, uint32_t color, size_t x, size_t y) {
    draw_char(t_fb, c, x * 8, y * 8, color);
}

void t_scroll(int lines) {
    if (lines <= 0) return;

    size_t rows = t_fb->height / 8;
    size_t cols = t_fb->width / 8;

    for (size_t y = lines; y < rows; y++) {
        for (size_t x = 0; x < cols; x++) {
            for (size_t py = 0; py < 8; py++) {
                for (size_t px = 0; px < 8; px++) {
                    volatile uint32_t *fb_ptr = t_fb->address;
                    size_t src_offset = ((y * 8 + py) * (t_fb->pitch / 4)) + (x * 8 + px);
                    size_t dst_offset = (((y - lines) * 8 + py) * (t_fb->pitch / 4)) + (x * 8 + px);
                    fb_ptr[dst_offset] = fb_ptr[src_offset];
                }
            }
        }
    }

    for (size_t y = rows - lines; y < rows; y++) {
        for (size_t x = 0; x < cols; x++) {
            for (size_t py = 0; py < 8; py++) {
                for (size_t px = 0; px < 8; px++) {
                    draw_pixel(t_fb, x * 8 + px, y * 8 + py, 0);
                }
            }
        }
    }

    t_row -= lines;
}

void t_delete_last_line() {
    size_t rows = t_fb->height / 8;
    size_t cols = t_fb->width / 8;

    for (size_t x = 0; x < cols; x++) {
        for (size_t py = 0; py < 8; py++) {
            for (size_t px = 0; px < 8; px++) {
                draw_pixel(t_fb, x * 8 + px, (rows - 1) * 8 + py, 0);
            }
        }
    }
}

void t_putchar(char c) {
    if (c == '\n') {
        t_col = 0;
        t_row++;
        if (t_row >= t_fb->height / 8) {
            t_scroll(1);
        }
        return;
    }
    
    t_putentryat(c, t_color, t_col, t_row);
    t_col++;
    
    if (t_col >= t_fb->width / 8) {
        t_col = 0;
        t_row++;
        if (t_row >= t_fb->height / 8) {
            t_scroll(1);
        }
    }
}

void t_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        t_putchar(data[i]);
    }
}

void t_wstring(const char* data) {
    while (*data) {
        t_putchar(*data);
        data++;
    }
}