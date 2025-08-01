#include "kernel.h"

#include <stdarg.h>

#include "idt.h"
#include "io.h"
#include "kheap.h"
#include "page.h"

static uint16_t cursor_row = 0;
static uint16_t cursor_col = 0;
static uint8_t default_clr = 2;

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

void putchar(char c) {
  if (c == '\n') {
    cursor_row++;
    cursor_col = 0;
  } else {
    char str[2] = {c, '\0'};
    write_chars(str, default_clr, cursor_col, cursor_row);
    cursor_col++;
  }
}

void print_int(int value) {
  char buf[12];
  int i = 0, is_negative = 0;
  if (value < 0) {
    is_negative = 1;
    value = -value;
  }
  do {
    buf[i++] = '0' + (value % 10);
    value /= 10;
  } while (value);
  if (is_negative) buf[i++] = '-';
  while (i--) putchar(buf[i]);
}

void printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  for (const char *p = fmt; *p; p++) {
    if (*p == '%') {
      p++;
      if (*p == 's') {
        char *s = va_arg(args, char *);
        while (*s) putchar(*s++);
      } else if (*p == 'd') {
        int d = va_arg(args, int);
        print_int(d);
      } else if (*p == '%') {
        putchar('%');
      }
    } else {
      putchar(*p);
    }
  }
  va_end(args);
}

static struct page_chunk_4gb *chunk = 0;

void kernel_main() {
  // uint8_t pos_x = VGA_WIDTH / 2 - 6; // (pos_x <= 80 - strlen)
  // uint8_t pos_y = VGA_HEIGHT / 2;    // (pos_y <= 20)
  // uint8_t text_clr = 2;              // green color
  uint8_t clear_clr = 0;  // default black clear color

  clear_screen(clear_clr);
  // write_chars("Hello World!", text_clr, 0,0);
  printf("Sup!\n");

  kheap_init();

  idt_init();

  // paging test
  chunk = page_new_chunk_4gb(PAGING_WRITABLE_SET | PAGING_PRESENT_SET |
                             PAGING_SUPERVISOR_SET);
  page_switch(page_chunk_get_pd_entries(chunk));
  enable_paging();

  enable_int();
}
