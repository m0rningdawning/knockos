#include "heap.h"

#include "config.h"
#include "memory.h"
#include "status.h"

static int heap_val_allign(void* p) {
  return ((unsigned int)p % KNOCKOS_HEAP_BLOCK_SIZE_T) == 0;
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

static uint32_t heap_allign_val(uint32_t val) {
  int rem = val % KNOCKOS_HEAP_BLOCK_SIZE_T;

  if (rem != 0) {
    val = (val - (val % KNOCKOS_HEAP_BLOCK_SIZE_T));
    val += KNOCKOS_HEAP_BLOCK_SIZE_T;
  }

  return val;
}

static int heap_get_entry_type(HEAP_B_TABLE_ENTRY entry) {
  return entry & 0x0f;
}

int heap_get_start_block(struct heap* heap, uint32_t total_blocks) {
  struct heap_table* table = heap->table;
  int bc = 0;
  int bs = -1;

  for (size_t i = 0; i < table->total; ++i) {
    if (heap_get_entry_type(table->entries[i]) != HEAP_B_TABLE_FREE) {
      bc = 0;
      bs = -1;
      continue;
    }

    if (bs == -1) {
      bs = i;
    }

    bc++;

    if (bc == total_blocks) {
      break;
    }
  }

  if (bc == -1) {
    return -ENOMEM;
  }

  return bs;
}

void* heap_block_to_addr(struct heap* heap, int block) {
  return heap->st_addr + (block * KNOCKOS_HEAP_BLOCK_SIZE_T);
}

void heap_mark_blocks_taken(struct heap* heap, int start_block,
                            int total_blocks) {
  int end_block = start_block + total_blocks - 1;

  HEAP_B_TABLE_ENTRY entry = HEAP_B_TABLE_TAKEN | HEAP_B_IS_FIRST;

  if (total_blocks > 1) {
    entry |= HEAP_B_HAS_NEXT;
  }

  for (int i = start_block; i <= end_block; ++i) {
    heap->table->entries[i] = entry;
    entry = HEAP_B_TABLE_TAKEN;

    if (i != end_block - 1) {
      entry |= HEAP_B_HAS_NEXT;
    }
  }
}

void* heap_malloc_blocks(struct heap* heap, uint32_t total_blocks) {
  void* addr = 0;

  int start_block = heap_get_start_block(heap, total_blocks);
  if (start_block < 0) {
    goto out;
  }

  addr = heap_block_to_addr(heap, start_block);

  heap_mark_blocks_taken(heap, start_block, total_blocks);

out:
  return addr;
}

void heap_mark_blocks_free(struct heap* heap, int start_block) {
  struct heap_table* table = heap->table;

  for (int i = start_block; i < (int)table->total; ++i) {
    HEAP_B_TABLE_ENTRY* entry = &table->entries[i];
    table->entries[i] = HEAP_B_TABLE_FREE;

    if (!(*entry & HEAP_B_HAS_NEXT)) {
      break;
    }
  }
}

int heap_addr_to_block(struct heap* heap, void* addr) {
  return (int)(addr - heap->st_addr) / KNOCKOS_HEAP_BLOCK_SIZE_T;
}

void* heap_malloc(struct heap* heap, size_t size) {
  size_t alligned_size = heap_allign_val(size);
  uint32_t total_blocks = alligned_size / KNOCKOS_HEAP_BLOCK_SIZE_T;
  return heap_malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap* heap, void* p) {
  heap_mark_blocks_free(heap, heap_addr_to_block(heap, p));
}
