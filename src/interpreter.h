#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>
#include <stdio.h>

int bf_run(FILE *fptr, int buffer_size, bool debug);

#endif
