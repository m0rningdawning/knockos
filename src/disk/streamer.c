#include "streamer.h"
#include "disk.h"
#include "kheap.h"
#include "config.h"

struct disk_stream* disk_stream_new(int id) {
  disk_s* disk = disk_get(id);
  if (!disk) return 0;

  struct disk_stream* stream = kzalloc(sizeof(struct disk_stream));
  stream->pos = 0;
  stream->disk = disk;
  return stream;
}

int disk_stream_seek(struct disk_stream* stream, int pos) {
  stream->pos = pos;
  return 0;
}

int disk_stream_read(struct disk_stream* stream, void* out, int total) {
  int sector = stream->pos / KNOCKOS_SECTOR_SIZE;
  int offset = stream->pos % KNOCKOS_SECTOR_SIZE;
  char buf[KNOCKOS_SECTOR_SIZE];

  int res = disk_read_block(stream->disk, sector, 1, buf);
  if (res < 0) goto out;

  int total_read = total > KNOCKOS_SECTOR_SIZE ? KNOCKOS_SECTOR_SIZE : total;
  for (int i = 0; i < total_read; ++i)
    *(char*)out++ = buf[offset + i];

  stream->pos += total_read;
  if (total > KNOCKOS_SECTOR_SIZE)
    res = disk_stream_read(stream, out, total - KNOCKOS_SECTOR_SIZE);

out:
  return res;
}

void disk_stream_close(struct disk_stream* stream) {
  kfree(stream);
}
