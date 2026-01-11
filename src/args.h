#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

typedef struct args {
  int buffer_size;
  bool dynamic_reallocation;
  bool json;
  bool debug;
  int pos_c;
  char **pos_v;
} args;

int parse_args(int argc, char **argv, args *args);
void free_args(args *args);

#endif
