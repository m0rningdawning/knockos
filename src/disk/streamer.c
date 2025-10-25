#include "streamer.h"
#include "kheap.h"

struct disk_stream* disk_stream_new(int id) {
  disk_s* disk = disk_get(id);
  if (!disk) return 0;

  struct disk_stream* stream = kzalloc(sizeof(struct disk_stream));
  stream->pos = 0;
  stream->disk = disk;
  return stream;
}
