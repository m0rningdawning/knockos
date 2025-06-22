#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void kernel_main();
size_t strlen(const char *str);
uint16_t make_char(const char c, const uint8_t color);
void clear_screen(const uint8_t color);
void write_chars(const char *string, const uint8_t color, const uint8_t pos_x,
                 const uint8_t pos_y);

#endif /* ifndef KERNEL_H */
