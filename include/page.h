#ifndef PAGE_H_
#define PAGE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PAGING_DIRECTORY_ENTRIES 1024
#define PAGING_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096

#define PAGING_CACHE_DISABLED_SET 0b00010000
#define PAGING_WRITE_THROUGH_SET  0b00001000
#define PAGING_SUPERVISOR_SET     0b00000100
#define PAGING_WRITABLE_SET       0b00000010
#define PAGING_PRESENT_SET        0b00000001

struct page_chunk_4gb{
  uint32_t* pd_entries;
};

void page_switch(uint32_t* pd_entries);
struct page_chunk_4gb* page_new_chunk_4gb(uint8_t flags);
uint32_t* page_chunk_get_pd_entries(struct page_chunk_4gb* chunk);
void enable_paging();
bool paging_is_alligned(void* addr);
int paging_get_idx(void* v_address, uint32_t* dir_idx_out, uint32_t* tab_idx_out);

#endif /* ifndef PAGE_H_ */
