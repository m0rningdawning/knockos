#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

size_t strlen(const char *str);
size_t strnlen(const char *str, int max);
int isdigit(char c);
int tonum(char c);

#endif /* ifndef STRING_H_ */
