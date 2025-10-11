#include "string.h"

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

size_t strnlen(const char *str, int max) {
  size_t len;
  for (len = 0; len < max; ++len) {
    if (str[0] == 0) break;
  }
  return len;
}

int isdigit(char c) {
  return c >= 48 && c <= 57;
}

int tonum(char c) {
  return c - 48;
}
