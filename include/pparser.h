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

#endif /* ifndef PPARSER_H_ */
