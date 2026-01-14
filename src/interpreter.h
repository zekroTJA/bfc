#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

int bf_run(char *fptr, int buffer_size, bool dynamic_realloc, bool output_json,
           bool debug, bool deny_input, char *sinput);

#endif
