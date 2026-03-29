/*
  ksclib-string.c

  The KCL String module provides an implementation of a string
  structure that tracks string length, provides support for string
  slices and uses KCL Arena for memory.

  Standard function prefix:  kcl_str_

*/

enum kcl_lst_type {
	VARRAY,
	LNKLST
};

typedef struct kcl_str_obj kcl_str;

struct kcl_str_obj {
	char*  str;
	unsigned len;
	unsigned size;
};

[[maybe_unused]]
static kcl_str *
kcl_str_new(const char* new_str, unsigned str_size, struct kcl_arena *arena)
{
	kcl_str* new_kcl_str = kcl_arn_push(arena, sizeof new_kcl_str);
	if (new_kcl_str) {
		new_kcl_str->str = kcl_arn_push(arena, str_size);
		if (new_kcl_str->str) {
			new_kcl_str->size = str_size;
			size_t i = 0;
			while (i < str_size && new_str[i]) {
				new_kcl_str->str[i] = new_str[i];
				i++;
			}
			new_kcl_str->len = i;
			return (new_kcl_str);
		}
	}
	return nullptr;
}

[[maybe_unused]]
static kcl_str *
kcl_str_set_value(kcl_str* str, const char* new_val)
{
	size_t i = 0;
	while (i < str->size && new_val[i]) {
		str->str[i] = new_val[i];
		i++;
	}
	str->len = i;
	return (str);
}

[[maybe_unused]]
static kcl_str *
kcl_str_slice_new(kcl_str* str, size_t start, size_t len, struct kcl_arena *arena)
{
	if ((start + len) > str->size) {
		return (nullptr);
	} else {
		kcl_str* new_kcl_str = kcl_arn_push(arena, sizeof new_kcl_str);
		new_kcl_str->str = str->str + start;
		new_kcl_str->len = len;
		new_kcl_str->size = 0; // 0 size to indicate a read only slice??
		return (new_kcl_str);
	}
}

[[maybe_unused]]
static kcl_str *
kcl_str_slice(kcl_str* slice, kcl_str* str, size_t start, size_t len)
{
	if ((start + len) > str->size) {
		return (nullptr);
	} else {
		slice->str = str->str + start;
		slice->len = len;
		slice->size = 0; // 0 size to indicate a read only slice??
		return (slice);
	}
}

[[maybe_unused]]
static size_t
kcl_str_len(kcl_str* str)
{
	return (str->len);
}

[[maybe_unused]]
static char*
kcl_str_cstr_alloc(kcl_str* str)
{
	return (strndup(str->str, str->len));
}

[[maybe_unused]]
static char*
kcl_str_to_cstr_new(kcl_str* str, kcl_arena* arena)
{
	char* cstr = kcl_arn_push(arena, str->len + 1);
	for (unsigned i = 0; i < str->len; i++) {
		cstr[i] = str->str[i];
	}
	cstr[str->len] = 0;
	return (cstr);
}

[[maybe_unused]]
void
kcl_str_fputs(kcl_str* str, FILE* stream)
{
	for (unsigned i = 0; i < str->len; i++) {
		fputc(str->str[i], stream);
	}
}

[[maybe_unused]]
static kcl_str *
kcl_str_concat_new(const char* str1, size_t str1_len, const char* str2, size_t str2_len, struct kcl_arena *arena)
{
	size_t str_len = str1_len + str2_len;
	kcl_str* new_kcl_str = kcl_arn_push(arena, sizeof new_kcl_str);
	if (new_kcl_str) {
		new_kcl_str->str = kcl_arn_push(arena, str_len);
		if (new_kcl_str->str) {
			new_kcl_str->size = str_len;
			new_kcl_str->len = str_len;
			size_t i, j;
			for (i = 0; i < str1_len; i++) {
				new_kcl_str->str[i] = str1[i];
			}
			for (j = 0; i < str_len; i++, j++) {
				new_kcl_str->str[i] = str2[j];
			}
			return (new_kcl_str);
		}
	}
	return nullptr;
}

[[maybe_unused]]
static kcl_str *
kcl_str_set_concat(kcl_str* str, const char* str1, size_t str1_len, const char* str2, size_t str2_len)
{
	size_t str_len = str1_len + str2_len;
	if (str_len > str->size) {
		return (nullptr);
	} else {
		size_t i, j;
		for (i = 0; i < str1_len; i++) {
			str->str[i] = str1[i];
		}
		for (j = 0; i < str_len; i++, j++) {
			str->str[i] = str2[j];
		}
		str->len = str_len;
		return (str);
	}
}

#define kcl_str_append(a, b)			\
	_Generic((b),				\
		 kcl_str*: kcl_str_append_kstr,	\
		 char*: kcl_str_append_cstr)	\
	(a, b)

[[maybe_unused]]
static bool
kcl_str_append_kstr(kcl_str* str, kcl_str* str2)
{
	if (str->size < (str->len + str2->len)) {
		return (false);
	} else {
		for (unsigned i = 0; i < str2->len; i++) {
			str->str[i + str->len] = str2->str[i];
		}
		str->len += str2->len;
		return (true);
	}
}

[[maybe_unused]]
static bool
kcl_str_append_cstr(kcl_str* str, const char* str2)
{
	int str2_len = strlen(str2);
	if (str->size < (str->len + str2_len)) {
		return (false);
	} else {
		for (unsigned i = 0; i < str2_len; i++) {
			str->str[i + str->len] = str2[i];
		}
		str->len += str2_len;
		return (true);
	}
}

#define kcl_str_find(a, b, c, d)		\
	_Generic((c),				\
		 char*: kcl_str_find_cstr,	\
		 int: kcl_str_find_char)	\
	(a, b, c, d)

[[maybe_unused]]
static bool
kcl_str_find_cstr(kcl_str* str, unsigned start_pos, char* query, unsigned* result_pos)
{
	size_t query_len = strlen(query);
	for (unsigned i = start_pos; i < str->len; i++) {
		if (str->str[i] == query[0]) {
			for (unsigned j = 1; j < query_len; j++) {
				if (str->str[i + j] != query[j]) {
					return false;
				}
			}
			*result_pos = i;
			return true;
		}
	}
	return false;
}

[[maybe_unused]]
static bool
kcl_str_find_char(kcl_str* str, unsigned start_pos, int query, unsigned* result_pos)
{
	for (unsigned i = start_pos; i < str->len; i++) {
		if (str->str[i] == query) {
			*result_pos = i;
			return true;
		}
	}
	return false;
}
