/*
  KigraSoft C Library

  ksclib.c
  
  Copyright (C) 2026 Stephen R. Kifer

  Situsest is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  KigraSoft C Library is distributed in the hope that it will be
  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Situsest.  If not, see <https://www.gnu.org/licenses/>.
*/

/**
  \file ksclib.c
  \brief KigraSoft C Library

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
#include "src/ksclib-string.c"
#include "src/ksclib-lists.c"

