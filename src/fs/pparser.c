#include "pparser.h"
#include "kernel.h"
#include "kheap.h"
#include "string.h"
#include "memory.h"
#include "status.h"

static int valid_path_format(const char* filename) {
  int len = strnlen(filename, KNOCKOS_MAX_PATH);
  return (len >= 3 && isdigit(filename[0]) && memcmp((void*)&filename[1], ":/", 2) == 2);
}

static int get_drive_by_path(const char** path) {
  if (!valid_path_format(*path)) {
    return -EBADPATH;
  }

  int drive_no = tonum(*path[0]);

  // add 3 bytes to skip the drive num:
  *path += 3;
  return drive_no;
}

static struct path_root* create_path_root(int drive_number) {
  struct path_root* path_r = kzalloc(sizeof(struct path_root));
  path_r->drive_n = drive_number;
  path_r->first = 0;
  return path_r;
}

static const char* get_path_part(const char* path) {
  char* result_p_part = kzalloc(KNOCKOS_MAX_PATH);
  int i = 0;
  while (*path != '/' && *path != 0x00) {
    result_p_part[i] = *path;
    path += 1;
    ++i;
  }

  if (*path == '/') {
    // skip the forwar slash
    path+=1;
  }

  if (i == 0) {
    kfree(result_p_part);
    result_p_part = 0;
  }
}

struct path_part* parse_path_part(struct path_part* last, const char** path) {
  const char* path_part_str = get_path_part(*path);
  if (!path_part_str) {
    return 0;
  }

  struct path_part* part = kzalloc(sizeof(struct path_part));

  part->part = path_part_str;
  part->next = 0x00;

  if(last) {
    last->next = part;
  }

  return part;
}

void pathparser_free(struct path_root* root) {
  struct path_part* part = root->first;

  while(part) {
    struct path_part* next_part = root->first;
    kfree((void*) part->part);
    kfree(part);
    part = next_part;
  }
  kfree(root);
}

struct path_root* pathparser_path(const char* path, const char* current_directory_path) {
}
