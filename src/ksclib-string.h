/*
  ksclib-string.h

  The KCL String module provides an implementation of a string
  structure that tracks string length, provides support for string
  slices and uses KCL Arena for memory.

  Standard function prefix:  kcl_str_
  
*/

#ifndef KCL_STRING
#define KCL_STRING

typedef struct kcl_str_obj kcl_str;

struct kcl_str_obj {
	char*  str;
	unsigned len;
	unsigned size;
};

static kcl_str * kcl_str_new(const char* new_str, unsigned str_size, struct kcl_arena *arena);
static kcl_str * kcl_str_set_value(kcl_str* str, const char* new_val);
static kcl_str * kcl_str_slice_new(kcl_str* str, size_t start, size_t len, struct kcl_arena *arena);
static kcl_str * kcl_str_slice(kcl_str* slice, kcl_str* str, size_t start, size_t len);
inline static size_t kcl_str_len(kcl_str* str);
inline static char kcl_str_get_char(kcl_str* str, unsigned position);
static char * kcl_str_cstr_alloc(kcl_str* str);
static char * kcl_str_to_cstr_new(kcl_str* str, kcl_arena* arena);
void kcl_str_fputs(kcl_str* str, FILE* stream);

static kcl_str * kcl_str_concat_new(const char* str1, size_t str1_len, const char* str2, size_t str2_len, struct kcl_arena *arena);
static kcl_str * kcl_str_set_concat(kcl_str* str, const char* str1, size_t str1_len, const char* str2, size_t str2_len);
#define kcl_str_append(a, b)			\
	_Generic((b),				\
		 kcl_str*: kcl_str_append_kstr,	\
		 char*: kcl_str_append_cstr)	\
	(a, b)

static bool kcl_str_append_kstr(kcl_str* str, kcl_str* str2);
static bool kcl_str_append_cstr(kcl_str* str, const char* str2);
#define kcl_str_find(a, b, c, d)		\
	_Generic((c),				\
		 char*: kcl_str_find_cstr,	\
		 int: kcl_str_find_char)	\
	(a, b, c, d)

static bool kcl_str_find_cstr(kcl_str* str, unsigned start_pos, char* query, unsigned* result_pos);
static bool kcl_str_find_char(kcl_str* str, unsigned start_pos, int query, unsigned* result_pos);
static bool kcl_str_equal(kcl_str* str1, kcl_str* str2);
static void kcl_str_trim(kcl_str* str);

// Declaration in ksclib-lists.h
// static kcl_list * kcl_str_get_lines(kcl_str* str, kcl_arena* arena);
	
#endif /* ndef KCL_STRING */
