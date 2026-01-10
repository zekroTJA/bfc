#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "args.h"
#include "errs.h"
#include "interpreter.h"
#include "output.h"

int main(int argc, char **argv) {
  int err = 0;

  args args = {};
  err = parse_args(argc, argv, &args);
  if (err < 0) {
    return 0;
  }
  if (err > 0) {
    goto cleanup;
  }

  FILE *finput;
  if (args.pos_c > 0) {
    finput = fopen(args.pos_v[0], "r");
    if (finput == NULL) {
      err = ERR_OPENING_INPUT_FILE;
      errorf("failed opening input file %s", args.pos_v[0]);
      goto cleanup;
    }
  } else {
    finput = stdin;
  }

  err = bf_run(finput, args.buffer_size, args.debug);

cleanup:
  free_args(&args);
  return err;
}
