#include "../include/kernel.h"

uint16_t make_char(const char c, const uint8_t color) {
  return (color << 8) | (uint8_t)c;
}

void clear_screen(const uint8_t color) {
  volatile uint16_t *video = (volatile uint16_t *)0xB8000;

  for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
    *video++ = make_char(' ', color);
  }
}

void write_chars(const char *string, const uint8_t color, const uint8_t pos_x,
                 const uint8_t pos_y) {
  volatile uint16_t *video =
      (volatile uint16_t *)0xB8000 + VGA_WIDTH * pos_y + pos_x;

  while (*string) {
    if (*string == '\n') {
      video += VGA_WIDTH - ((video - (volatile uint16_t *)0xB8000) % VGA_WIDTH);
      (void)*string++;
    }
    *video++ = make_char(*string++, color);
  }
}

void kernel_main() {
  uint8_t pos_x = 0;     // (default == 80)
  uint8_t pos_y = 0;     // (default == 20)
  uint8_t text_clr = 2;  // green color
  uint8_t crear_clr = 0; // default black clear color

  clear_screen(crear_clr);
  write_chars("Hello\nWorld!", text_clr, pos_x, pos_y);
}

