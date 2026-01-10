#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "errs.h"
#include "output.h"

// Here comes some obscure macro wizardry to get the maxmum value of any opaque
// type.
// Why? Because.
// Source - https://stackoverflow.com/a/7266266
// Posted by Glyph, modified by community. See post 'Timeline' for change
// history Retrieved 2026-01-10, License - CC BY-SA 4.0
#define issigned(t) (((t)(-1)) < ((t)0))
#define umaxof(t)                                                              \
  (((0x1ULL << ((sizeof(t) * 8ULL) - 1ULL)) - 1ULL) |                          \
   (0xFULL << ((sizeof(t) * 8ULL) - 4ULL)))
#define smaxof(t)                                                              \
  (((0x1ULL << ((sizeof(t) * 8ULL) - 1ULL)) - 1ULL) |                          \
   (0x7ULL << ((sizeof(t) * 8ULL) - 4ULL)))
#define maxof(t) ((unsigned long long)(issigned(t) ? smaxof(t) : umaxof(t)))

// We define the cell size to be an 8bit integer (char).
typedef unsigned char CELL;
const CELL MAX_CELL = maxof(CELL);

#define MAX_LOOP_DEPTH 1000

// Valid tokens
#define C_PTR_INC '>'
#define C_PTR_DEC '<'
#define C_VAL_INC '+'
#define C_VAL_DEC '-'
#define C_PRINT '.'
#define C_LOOP_START '['
#define C_LOOP_END ']'

typedef struct scanner {
  char *src;
  int cursor;
} scanner;

char scanner_next(scanner *s) {
  char c = s->src[s->cursor];
  if (c == '\0') {
    return EOF;
  }

  s->cursor++;

  return c;
}

void scanner_reset(scanner *s, int to) {
  assert(s->cursor > 0);
  s->cursor = to;
}

void debug_print(int pointer, int buffer_size, CELL *buffer, int idx) {
  assert(buffer != NULL);

  fprintf(stderr, "pointer: %d\n", pointer);
  fprintf(stderr, "index:   %d\n", idx);
  fprintf(stderr, "[%d", buffer[0]);
  for (int i = 1; i < buffer_size; i++) {
    fprintf(stderr, ", %d", buffer[i]);
  }
  fprintf(stderr, "]\n");
}

int bf_run(char *sinput, int buffer_size, bool debug) {
  assert(buffer_size > 0);
  assert(sinput != NULL);

  int err = 0;

  int loop_stack[MAX_LOOP_DEPTH] = {0};
  int loop_stack_head = 0;

  CELL *buffer = (CELL *)malloc(buffer_size * sizeof(CELL));
  if (buffer == NULL) {
    err = ERR_CELL_BUFFER_ALLOCATION_FAILED;
    errorf("cell buffer allocation failed");
    goto cleanup;
  }
  // Is this necessary?
  memset(buffer, (CELL)0, buffer_size * sizeof(CELL));

  int pointer = 0;

  scanner sc = {.src = sinput, .cursor = 0};

  char c = 0;
  while ((c = scanner_next(&sc)) != EOF) {
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

    case C_LOOP_START:
      if (buffer[pointer] == 0) {
        int loop_depth = 1;
        while ((c = scanner_next(&sc)) != EOF) {
          if (c == C_LOOP_START) {
            loop_depth++;
          } else if (c == C_LOOP_END && --loop_depth == 0) {
            --loop_stack_head;
            break;
          }
        }
      }
      loop_stack[loop_stack_head++] = sc.cursor - 1;
      break;

    case C_LOOP_END:
      scanner_reset(&sc, loop_stack[--loop_stack_head]);
      break;

    default:
      // NOOP
      break;
    }
  }

  if (loop_stack_head > 0) {
    err = ERR_UNMATCHED_LOOP;
    errorf("unmatched end of loop");
    goto cleanup;
  }

cleanup:
  if (debug) {
    debug_print(pointer, buffer_size, buffer, sc.cursor);
  }
  free(buffer);
  return err;
}
