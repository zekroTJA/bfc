#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "errs.h"
#include "output.h"

const int DEFAULT_BUFFER_SIZE = 30000;

void print_help() {
  fputs("usage: bfc [options] [INPUT_FILE]\n", stderr);
  fputs("\n", stderr);
  fputs("Arguments:\n", stderr);
  fputs("\tINPUT_FILE  Source file (reads from stdin if not specified)\n",
        stderr);
  fputs("\n", stderr);
  fputs("Options:\n", stderr);
  fprintf(stderr,
          "\t-b, --buffer-size <BUFFER_SIZE>  Define the size of the program "
          "buffer [default: %d]\n",
          DEFAULT_BUFFER_SIZE);
  fputs("\t-d, --debug                      Enable debug mode\n", stderr);
  fputs("\t-h, --help                       Print help message\n", stderr);
}

int parse_args(int argc, char **argv, args *args) {
  // Set defaults:
  args->buffer_size = DEFAULT_BUFFER_SIZE;

  // Early return when no args were passed.
  if (argc < 2) {
    return 0;
  }

  int pos_c = 0;
  char **pos_v = malloc(argc * sizeof(char *));

  for (int i = 1; i < argc; i++) {
    char *curr = argv[i];

    if (strcmp(curr, "--help") == 0 || strcmp(curr, "-h") == 0) {
      print_help();
      return -1;
    }

    if (strcmp(curr, "--buffer-size") == 0 || strcmp("-b", curr) == 0) {
      if (++i >= argc) {
        errorf("argument --buffer-size requires a value");
        return ERR_INVALID_ARGUMENT;
      }
      if ((args->buffer_size = atoi(argv[i])) <= 0) {
        errorf("invalid value for argument --buffer-size: %s", argv[i]);
        return ERR_INVALID_ARGUMENT;
      }
      continue;
    }

    if (strcmp(curr, "--debug") == 0 || strcmp(curr, "-d") == 0) {
      args->debug = true;
      continue;
    }

    // Because pos_v is allocated with size of argc, should
    // pos_c be larger or equal argc, catch that here.
    assert(pos_c < argc);
    pos_v[pos_c++] = curr;
  }

  args->pos_c = pos_c;
  args->pos_v = pos_v;

  return 0;
}

void free_args(args *args) { free(args->pos_v); }
