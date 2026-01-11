#ifndef DSTRING_H
#define DSTRING_H

typedef struct dstring {
  char *buffer;
  unsigned long length;
  unsigned long capacity;
} dstring;

int dstring_push_char(dstring *ds, char c);
int dstring_push_char_escaped(dstring *ds, char c);
void dstring_free(dstring *ds);

#endif
