#include "kheap.h"

#include "config.h"
#include "heap.h"
#include "kernel.h"
#include "memory.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init() {
  int table_entries = KNOCKOS_HEAP_SIZE_T / KNOCKOS_HEAP_BLOCK_SIZE_T;
  kernel_heap_table.entries = (HEAP_B_TABLE_ENTRY*)KNOCKOS_HEAP_TABLE_ADDR;
  kernel_heap_table.total = table_entries;

  void* end = (void*)(KNOCKOS_HEAP_START_ADDR + KNOCKOS_HEAP_SIZE_T);
  int res = heap_create(&kernel_heap, (void*)(KNOCKOS_HEAP_START_ADDR), end,
                        &kernel_heap_table);

  if (res < 0) {
    printf("Heap creation error!\n");
  }
}

void* kmalloc(size_t size) { return heap_malloc(&kernel_heap, size); }

void* kzalloc(size_t size) {
  void* p = kmalloc(size);
  if (p < 0) {
    printf("kzalloc error!\n");
    return 0;
  }
  memset(p, 0x00, size);
  return p;
}

void kfree(void* p) { heap_free(&kernel_heap, p); }
