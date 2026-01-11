#include <assert.h>
#include <stdlib.h>

#include "dstring.h"
#include "errs.h"

const unsigned long INIT_CAPACITY = 1024;

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

void dstring_free(dstring *ds) {
  assert(ds != NULL);

  free(ds->buffer);
  ds->buffer = NULL;
  ds->length = 0;
  ds->capacity = 0;
}
