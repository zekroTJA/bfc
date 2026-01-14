#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

typedef struct args {
  int buffer_size;
  bool debug;
  bool deny_input;
  bool dynamic_reallocation;
  char *input;
  bool json;
  int pos_c;
  char **pos_v;
} args;

int parse_args(int argc, char **argv, args *args);
void free_args(args *args);

#endif
