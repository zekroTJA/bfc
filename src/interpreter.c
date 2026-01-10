#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "errs.h"
#include "output.h"

// Here comes some obscure macro wizardry to get the maxmum value of any opaque
// type.
// Why? Because.
// Source: https://stackoverflow.com/a/7266266
#define issigned(t) (((t)(-1)) < ((t)0))
#define umaxof(t)                                                              \
  (((0x1ULL << ((sizeof(t) * 8ULL) - 1ULL)) - 1ULL) |                          \
   (0xFULL << ((sizeof(t) * 8ULL) - 4ULL)))
#define smaxof(t)                                                              \
  (((0x1ULL << ((sizeof(t) * 8ULL) - 1ULL)) - 1ULL) |                          \
   (0x7ULL << ((sizeof(t) * 8ULL) - 4ULL)))
#define maxof(t) ((unsigned long long)(issigned(t) ? smaxof(t) : umaxof(t)))

// We define the cell size to be an 8bit integer (char).
typedef char CELL;
const CELL MAX_CELL = maxof(CELL);

// Valid tokens
#define C_PTR_INC '>'
#define C_PTR_DEC '<'
#define C_VAL_INC '+'
#define C_VAL_DEC '-'
#define C_PRINT '.'
#define C_LOOP_START '['
#define C_LOOP_END ']'

void debug_print(int pointer, int cells_size, CELL *cell) {
  assert(cell != NULL);

  fprintf(stderr, "pointer: %d\n", pointer);
  fprintf(stderr, "[%d", cell[0]);
  for (int i = 1; i < cells_size; i++) {
    fprintf(stderr, ", %d", cell[i]);
  }
  fprintf(stderr, "]\n");
}

int bf_run(FILE *fptr, int buffer_size, bool debug) {
  assert(buffer_size > 0);
  assert(fptr != NULL);

  int err = 0;

  CELL *buffer = (CELL *)malloc(buffer_size * sizeof(CELL));
  if (buffer == NULL) {
    err = ERR_CELL_BUFFER_ALLOCATION_FAILED;
    errorf("cell buffer allocation failed");
    goto cleanup;
  }

  int pointer = 0;

  char c = 0;
  while ((c = fgetc(fptr)) > 0) {
    switch (c) {

    case C_PTR_INC:
      if (++pointer >= buffer_size) {
        err = ERR_BUFFER_INDEX_OUT_OF_BOUNDS;
        errorf("buffer index out of bounds (idx: %d >= cell size: %d)", pointer,
               buffer_size);
        goto cleanup;
      }
      break;

    case C_PTR_DEC:
      if (--pointer < 0) {
        err = ERR_BUFFER_INDEX_OUT_OF_BOUNDS;
        errorf("buffer index out of bounds (idx: %d >= cell size: %d)", pointer,
               buffer_size);
        goto cleanup;
      }
      break;

    case C_VAL_INC:
      if (buffer[pointer] + 1 > MAX_CELL) {
        buffer[pointer] = 0;
      } else {
        buffer[pointer]++;
      }
      break;

    case C_VAL_DEC:
      if (buffer[pointer] == 0) {
        buffer[pointer] = MAX_CELL;
      } else {
        buffer[pointer]--;
      }
      break;

    case C_PRINT:
      putc(buffer[pointer], stdout);
      break;

    default:
      // NOOP
      break;
    }
  }

cleanup:
  if (debug) {
    debug_print(pointer, buffer_size, buffer);
  }
  free(buffer);
  fclose(fptr);
  return err;
}
