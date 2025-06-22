#include "../include/kernel.h"

void write_string(const char *string, int color) {
  volatile char *video = (volatile char *)0xB8000;

  while (*string != 0) {
    *video++ = *string++;
    *video++ = color;
  }
}

void kernel_main() { write_string("Hello World!", 2); }

