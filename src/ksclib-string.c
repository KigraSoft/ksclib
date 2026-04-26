/*
  ksclib-string.c

  The KCL String module provides an implementation of a string
  structure that tracks string length, provides support for string
  slices and uses KCL Arena for memory.

  Standard function prefix:  kcl_str_

*/
/*
enum kcl_lst_type {
	VARRAY,
	LNKLST
};
*/
typedef struct kcl_str_obj kcl_str;

struct kcl_str_obj {
	char*  str;
	unsigned len;
	unsigned size;
};


//
// ** not good way to do this
//
enum kcl_lst_type {
	VARRAY,
	LNKLST,
	KV_STR
};
typedef struct kcl_lst_obj {
	void		*datum;
	struct kcl_lst_obj*	 next;
	struct kcl_lst_obj*	 prev;
	kcl_str*	 key_str;
} kcl_lst_obj;
typedef struct kcl_list {
	kcl_arena		*arena;
	kcl_lst_obj		*list;
	kcl_lst_obj		*current;
	enum kcl_lst_type	 type;
	unsigned int		 count;
	unsigned int		 size;
} kcl_list;

static struct kcl_list * kcl_lst_alloc_list(enum kcl_lst_type type, struct kcl_arena *arena, unsigned int num_elements);
static unsigned int kcl_lst_add_datum_w_key(struct kcl_list *list, void *datum, kcl_str* key);
static unsigned int kcl_lst_append_datum_w_key(struct kcl_list *list, void *datum, kcl_str* key);

//
// move to somewhere else
//


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
	if ((start + len) > str->len) {
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
	if ((start + len) > str->len) {
		return (nullptr);
	} else {
		slice->str = str->str + start;
		slice->len = len;
		slice->size = 0; // 0 size to indicate a read only slice??
		return (slice);
	}
}

[[maybe_unused]] inline static size_t
kcl_str_len(kcl_str* str)
{
	return (str->len);
}

[[maybe_unused]] inline static char
kcl_str_get_char(kcl_str* str, unsigned position)
{
	return str->str[position];
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

[[maybe_unused]]
static bool
kcl_str_equal(kcl_str* str1, kcl_str* str2)
{
	if (str1->len == str2->len) {
		for (unsigned i = 0; i < str1->len; i++) {
			if (str1->str[i] != str2->str[i]) {
				return false;
			}
		}
		return true;
	} else {
		return false;
	}
}

[[maybe_unused]]
static void
kcl_str_trim(kcl_str* str)
{
	unsigned start_posn = 0;
	unsigned end_posn = str->len - 1;
	unsigned new_len;
	while (str->str[start_posn] == ' ') {
		start_posn++;
	}
	while (str->str[end_posn] == ' ') {
		end_posn--;
	}
	new_len = end_posn - start_posn + 1;
	if (str->size > 0) { // if not string slice move text to beginning of str
		if (start_posn > 0) {
			for (unsigned i = 0; i < new_len; i++) {
				str->str[i] = str->str[i + start_posn];
			}
		}
	} else { // if string slice; just reposition pointer
		str->str = str->str + start_posn;
	}
	str->len = new_len;
}
	
[[maybe_unused]] static kcl_list*
kcl_str_get_lines(kcl_str* str, kcl_arena* arena)
{
	unsigned cur_posn = 0;
	unsigned qry_posn = 0;
	unsigned str_len = kcl_str_len(str);
	kcl_str* line;
	kcl_list* line_list = kcl_lst_alloc_list(LNKLST, arena, 0);
	if (!line_list) { return (nullptr); }
	//kcl_lst_add_datum(line_list, cur_posn);
	while (cur_posn < str_len) {
		if (kcl_str_find(str, cur_posn, '\n', &qry_posn)) {
			line = kcl_str_slice_new(str, cur_posn, qry_posn - cur_posn, arena);
			cur_posn = qry_posn + 1;
		} else {
			line = kcl_str_slice_new(str, cur_posn, str_len - cur_posn, arena);
		}
		if (line) {
			kcl_lst_append_datum_w_key(line_list, line, nullptr);
		} else {
			return (nullptr);
		}
	}
	return (line_list);
}

