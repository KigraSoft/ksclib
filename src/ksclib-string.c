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
	size_t len;
	size_t size;
};

[[maybe_unused]]
static kcl_str *
kcl_str_new(const char* new_str, size_t str_size, struct kcl_arena *arena)
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
