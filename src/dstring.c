#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "dstring.h"
#include "errs.h"

const unsigned long INIT_CAPACITY = 1024;

int push_escaped(dstring *ds, char c) {
  int err;
  if ((err = dstring_push_char(ds, '\\'))) {
    return err;
  }
  return dstring_push_char(ds, c);
}

int dstring_push_char(dstring *ds, char c) {
  assert(ds != NULL);
  assert(c > 0);

  if (ds->buffer == NULL) {
    ds->capacity = INIT_CAPACITY;
    ds->length = 0;
    ds->buffer = (char *)malloc(ds->capacity * sizeof(char));
    if (ds->buffer == NULL) {
      return ERR_DSTRING_ALLOC_FAILED;
    }
  } else if (ds->length >= ds->capacity) {
    ds->capacity *= 2;
    char *tmp = (char *)realloc(ds->buffer, ds->capacity * sizeof(char));
    if (tmp == NULL) {
      return ERR_DSTRING_ALLOC_FAILED;
    }
    ds->buffer = tmp;
  }

  ds->buffer[ds->length++] = c;
  ds->buffer[ds->length] = '\0';

  return 0;
}

int dstring_push_char_escaped(dstring *ds, char c) {
  switch (c) {
  case '"':
    return push_escaped(ds, '"');
  case '\\':
    return push_escaped(ds, '\\');
  case '\b':
    return push_escaped(ds, 'b');
  case '\f':
    return push_escaped(ds, 'f');
  case '\n':
    return push_escaped(ds, 'n');
  case '\r':
    return push_escaped(ds, 'r');
  case '\t':
    return push_escaped(ds, 't');
  }

  if (c < 0x20) {
    int err;
    char buf[7] = {'\\', 'u', '0', '0'};
    snprintf(buf + 4, 3, "%02X", c);
    for (unsigned long i = 0; i < sizeof(buf) && buf[i] != 0; i++) {
      if ((err = dstring_push_char(ds, buf[i]))) {
        return err;
      }
    }
    return 0;
  }

  return dstring_push_char(ds, c);
}

void dstring_free(dstring *ds) {
  assert(ds != NULL);

  free(ds->buffer);
  ds->buffer = NULL;
  ds->length = 0;
  ds->capacity = 0;
}
