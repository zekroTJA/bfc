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
  fputs("\t-D, --debug                      Enable debug mode\n", stderr);
  fputs("\t    --deny-input                 Throws an error when program tries "
        "to read input\n",
        stderr);
  fputs("\t-d, --dynamic-reallocation       Enable dynamic reallocation of the "
        "program buffer\n",
        stderr);
  fputs("\t-h, --help                       Print help message\n", stderr);
  fputs("\t-i, --input <INPUT>              Pass program input as string "
        "argument\n",
        stderr);
  fputs("\t-j, --json                       Output as JSON format\n", stderr);
}

int parse_args(int argc, char **argv, args *args) {
  // Set defaults:
  args->buffer_size = DEFAULT_BUFFER_SIZE;
  args->debug = false;
  args->dynamic_reallocation = false;
  args->input = NULL;
  args->json = false;

  // Early return when no args were passed.
  if (argc < 2) {
    return 0;
  }

  int pos_c = 0;
  char **pos_v = malloc(argc * sizeof(char *));

  for (int i = 1; i < argc; i++) {
    char *curr = argv[i];
    unsigned long curr_len = strlen(curr);

    if (curr_len == 0) {
      errorf("a value for [INPUT_FILE] is required but non was supplied");
      return ERR_INVALID_ARGUMENT;
    }

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

    if (strcmp(curr, "--debug") == 0 || strcmp(curr, "-D") == 0) {
      args->debug = true;
      continue;
    }

    if (strcmp(curr, "--deny-input") == 0) {
      args->deny_input = true;
      continue;
    }

    if (strcmp(curr, "--dynamic-reallocation") == 0 ||
        strcmp(curr, "-d") == 0) {
      args->dynamic_reallocation = true;
      continue;
    }

    if (strcmp(curr, "--input") == 0 || strcmp(curr, "-i") == 0) {
      if (++i >= argc) {
        errorf("argument --input requires a value");
        return ERR_INVALID_ARGUMENT;
      }
      args->input = argv[i];
      continue;
    }

    if (strcmp(curr, "--json") == 0 || strcmp(curr, "-j") == 0) {
      args->json = true;
      continue;
    }

    // Catching invalid arguments
    if (curr_len > 1 && curr[0] == '-') {
      errorf("invalid option '%s'", curr);
      return ERR_INVALID_ARGUMENT;
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

void free_args(args *args) {
  free(args->pos_v);
  args->pos_v = NULL;
}
