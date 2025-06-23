#include "../include/kernel.h"

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

uint16_t make_char(const char c, const uint8_t color) {
  return (color << 8) | (uint8_t)c;
}

void clear_screen(const uint8_t color) {
  volatile uint16_t *video = (volatile uint16_t *)0xB8000;

  for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; ++i) {
    *video++ = make_char(' ', color);
  }
}

void write_chars(const char *string, const uint8_t color, const uint8_t pos_x,
                 const uint8_t pos_y) {
  if ((pos_x + strlen(string) > VGA_WIDTH) || (pos_y > VGA_HEIGHT)) {
    write_chars("Error: Out of bounds", 4 /*red bg*/, 0, 0);
    return;
  }
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
  uint8_t pos_x = VGA_WIDTH / 2 - 6; // (pos_x <= 80 - strlen)
  uint8_t pos_y = VGA_HEIGHT / 2;    // (pos_y <= 20)
  uint8_t text_clr = 2;              // green color
  uint8_t crear_clr = 0;             // default black clear color

  clear_screen(crear_clr);
  write_chars("Hello World!", text_clr, pos_x, pos_y);
}

