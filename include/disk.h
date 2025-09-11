#ifndef DISK_H
#define DISK_H

typedef unsigned int KNOCKOS_DISK_TYPE;

#define KNOCKOS_DISK_TYPE_REAL 0

typedef struct {
  KNOCKOS_DISK_TYPE type;
  int sector_size;
} disk_s;

int disk_read_sector(int lba, int total, void* buff);
void disk_search_init();
disk_s* disk_get(int index);
int disk_read_block(disk_s *idisk, unsigned int lba, int total, void* buf);

#endif /* ifndef DISK_H */
