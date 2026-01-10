#include <stdarg.h>
#include <stdio.h>

void errorf(const char *format, ...) {
  va_list args;

  fprintf(stderr, "error: ");
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  fprintf(stderr, "\n");
}
