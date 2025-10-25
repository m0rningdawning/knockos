#ifndef D_STREAMER_H
#define D_STREAMER_H

#include "disk.h"

struct disk_stream {
  int pos;
  disk_s* disk;
};

#endif /* ifndef STREAMER */
