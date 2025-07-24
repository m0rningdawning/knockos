#ifndef HEAP_H_
#define HEAP_H_

#include "config.h"
#include <stdint.h>
#include <stddef.h>

typedef unsigned char HEAP_B_TABLE_ENTRY;

#define HEAP_B_TABLE_TAKEN 0x01
#define HEAP_B_TABLE_FREE  0x00

#define HEAP_B_HAS_NEXT 0b10000000
#define HEAP_B_IS_FREE  0b01000000

struct heap_table {
  HEAP_B_TABLE_ENTRY* entries;
  size_t total;
};

struct heap {
  struct heap_table* table;
  void* st_addr;
};

int heap_create(struct heap* heap, void* p, void* end, struct heap_table* table);

#endif /* ifndef HEAP_H_ */
