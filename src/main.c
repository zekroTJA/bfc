#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "errs.h"
#include "interpreter.h"
#include "output.h"

char *read_to_string(FILE *file) {
  assert(file != NULL);

  int capacity = 1024;
  int length = 0;

  char *buffer = malloc(capacity);
  if (!buffer) {
    errorf("failed allocating buffer for reading file");
    return NULL;
  }

  char c;
  while ((c = fgetc(file)) != EOF) {
    if (length + 1 >= capacity) {
      capacity *= 2;
      char *temp = realloc(buffer, capacity);
      if (!temp) {
        free(buffer);
        errorf("failed resizing buffer for reading file");
        return NULL;
      }
      buffer = temp;
    }
    buffer[length++] = c;
  }

  buffer[length] = '\0';
  return buffer;
}

int main(int argc, char **argv) {
  int err = 0;
  char *sinput = NULL;
  FILE *finput = NULL;

  args args = {};
  err = parse_args(argc, argv, &args);
  if (err < 0) {
    return 0;
  }
  if (err > 0) {
    goto cleanup;
  }

  if (args.pos_c > 0 && strcmp(args.pos_v[0], "-") != 0) {
    finput = fopen(args.pos_v[0], "r");
    if (finput == NULL) {
      err = ERR_OPENING_INPUT_FILE;
      errorf("failed opening input file %s", args.pos_v[0]);
      goto cleanup;
    }
  } else {
    finput = stdin;
  }

  sinput = read_to_string(finput);
  if (!sinput) {
    err = ERR_READING_INPUT;
    goto cleanup;
  }

  err = bf_run(sinput, args.buffer_size, args.dynamic_reallocation, args.json,
               args.debug, args.deny_input, args.input);

cleanup:
  free(sinput);
  if (finput != NULL && finput != stdin) {
    fclose(finput);
  }
  free_args(&args);
  return err;
}
