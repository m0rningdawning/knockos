#ifndef PPARSER_H_
#define PPARSER_H_

struct path_root {
  int drive_n;
  struct path_part* first;
};

struct path_part {
  const char* part;
  struct path_part* next;
};

struct path_root* pathparser_parse(const char* path, const char* current_directory_path);
void pathparser_free(struct path_root* root);

#endif /* ifndef PPARSER_H_ */
