/*
  ksclib.c

  KigraSoft C Library is a set of utility functions that I use in my
  other programs.  It is shared so that anyone else that finds it
  useful may also take advantage of it.

  Principles:
  - It is intended to be included directly for compilation as a single translation unit
  - It uses its internal memory arena structures where possible
  - All functions will be named in a 'kcl_xxx_' pattern where xxx is a code for the different main library 'modules'.
  - It will probably be C23 standard compliant
  - ???
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "src/ksclib-arena.c"
#include "src/ksclib-lists.c"
#include "src/ksclib-string.c"

