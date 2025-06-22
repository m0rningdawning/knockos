#include "../include/kernel.h"

void clear_screen(unsigned short color, const unsigned short w,
                  const unsigned short h) {
  volatile unsigned short *video = (unsigned short *)0xB8000;

  for (int i = 0; i < w * h; i++) {
    *video++ = ' ';
    *video++ = color;
  }
}

void write_string(const char *string, const unsigned short color,
                  const unsigned short pos_x, const unsigned short pos_y) {
  volatile char *video = (volatile char *)0xB8000 + 320 * pos_y + pos_x;

  while (*string) {
    *video++ = *string++;
    *video++ = color;
  }
}

void kernel_main() {
  const short text_clr = 2;  // green color
  const short crear_clr = 0; // default black clear color
  const short pos_x = 70;    // (default == 140) / 2 = 70
  const short pos_y = 6;     // (default == 12) / 2 = 6
  const short w = 140;
  const short h = 12;

  clear_screen(crear_clr, w, h);
  write_string("Hello World!", text_clr, pos_x, pos_y);
}

