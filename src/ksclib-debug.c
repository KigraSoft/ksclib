/*
  ksclib-debug.c

  The KCL Debug module provides various utilities to help with debugging.

  Standard function prefix:  kcl_dbg_

*/

#define kcl_dbg_printvar(s, x) \
	_Generic((x),				     \
		 unsigned *: kcl_dbg_print_uint_ptr, \
		 int *:      kcl_dbg_print_int_ptr,  \
		 char *:     kcl_dbg_print_str)      \
	(s, x, __FILE__, __LINE__)

void
kcl_dbg_print_uint_ptr(char* s, unsigned* x, char* f, int l) {
	printf("File/Line: %s/%i\t%s: %u - %p\n", f, l, s, *x, x);
}

void
kcl_dbg_print_int_ptr(char* s, int* x, char* f, int l) {
	printf("File/Line: %s/%i\t%s: %i - %p\n", f, l, s, *x, x);
}

void
kcl_dbg_print_str(char* s, char* x, char* f, int l) {
	printf("File/Line: %s/%i\t%s: %s - %p\n", f, l, s, x, x);
}

