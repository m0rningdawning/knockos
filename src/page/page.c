#include "page.h"
#include "kheap.h"

static uint32_t* curr_pd_entry = 0;

void page_load_pd_entry(uint32_t* pd_entries);

struct page_chunk_4gb* page_new_chunk_4gb(uint8_t flags) {
  uint32_t* pd_entries = kzalloc(sizeof(uint32_t) * PAGING_DIRECTORY_ENTRIES);
  int offset = 0;

  for (int i = 0; i < PAGING_DIRECTORY_ENTRIES; ++i) {
    uint32_t* pt_page = kzalloc(sizeof(uint32_t) * PAGING_ENTRIES_PER_TABLE);

    for (int j = 0; j < PAGING_ENTRIES_PER_TABLE; ++j) {
      pt_page[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
    }

    offset += (PAGING_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
    pd_entries[i] = (uint32_t)pt_page | flags | PAGING_WRITABLE_SET;
  }

  struct page_chunk_4gb* chunk = kzalloc(sizeof(struct page_chunk_4gb));

  chunk->pd_entries = pd_entries;
  return chunk;
}

void page_switch(uint32_t* pd_entries) {
  page_load_pd_entry(pd_entries);
  curr_pd_entry = pd_entries;
}

uint32_t* page_chunk_get_pd_entries(struct page_chunk_4gb* chunk) {
  return chunk->pd_entries;
}
