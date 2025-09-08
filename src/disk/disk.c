#include "io.h"

int disk_read_sector(int lba, int total, void* buff) {
  outb(0x1F6, (lba >> 24) | 0xE0);
  outb(0x1F2, total);
  outb(0x1F3, (unsigned char)(lba & 0xFF));
  outb(0x1F4, (unsigned char)(lba >> 8));
  outb(0x1F5, (unsigned char)(lba >> 16));
  outb(0x1F7, 0x20);

  unsigned short* ptr = (unsigned short*) buff;

  for (int b = 0; b < total; ++b) {
    char c = insb(0x1F7);

    while(!(c & 0x08)) {
      c = insb (0x1F7);
    }

    for (int i = 0; i < 256; ++i) {
      *ptr = insw(0x1F0);
      ptr++;
    }
  }

  return 0;
}
