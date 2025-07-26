#include "heap.h"

#include "config.h"
#include "memory.h"
#include "status.h"

static int heap_val_allign(void* ptr) {
  return ((unsigned int)ptr % KNOCKOS_HEAP_BLOCK_SIZE_T) == 0;
}

static int heap_val_table(void* p, void* end, struct heap_table* table) {
  int res = 0;

  size_t table_size = (size_t)(end - p);
  size_t total_blocks = table_size / KNOCKOS_HEAP_BLOCK_SIZE_T;

  if (table->total != total_blocks) {
    res = -EINVARG;
    goto out;
  }
out:
  return res;
}

int heap_create(struct heap* heap, void* p, void* end,
                struct heap_table* table) {
  int res = 0;

  if (!heap_val_allign(p) || !heap_val_allign(end)) {
    res = -EINVARG;
    goto out;
  }

  memset(heap, 0, sizeof(struct heap));
  heap->st_addr = p;
  heap->table = table;

  res = heap_val_table(p, end, table);
  if (res < 0) {
    goto out;
  }

  size_t table_size = sizeof(HEAP_B_TABLE_ENTRY) * table->total;
  memset(table->entries, HEAP_B_TABLE_FREE, table_size);

out:
  return res;
}
