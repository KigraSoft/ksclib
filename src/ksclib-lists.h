/*
  ksclib-lists.h

  The KCL Lists module provides an implementation of various dynamic
  list types using the KCL Arena library for memory.

  Standard function prefix:  kcl_lst_

*/

#ifndef KCL_LISTS
#define KCL_LISTS

#include "ksclib-string.h"

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
#define kcl_lst_add_datum(a, b) kcl_lst_add_datum_w_key(a, b, nullptr)
static unsigned int kcl_lst_append_datum_w_key(struct kcl_list *list, void *datum, kcl_str* key);
#define kcl_lst_append_datum(a, b) kcl_lst_append_datum_w_key(a, b, nullptr)
static bool kcl_lst_del_datum(struct kcl_list *list, void *datum);
static void * kcl_lst_get_first(struct kcl_list *list);
static void * kcl_lst_get_next(struct kcl_list *list);
static kcl_str * kcl_lst_get_cur_key(struct kcl_list *list);
static void * kcl_lst_get_val(kcl_list* list, kcl_str* key);
static bool kcl_lst_set_element(struct kcl_list *list, void *datum, unsigned int index);
static void * kcl_lst_get_element(struct kcl_list *list, unsigned int index);

// From ksclib-string.c
static kcl_list * kcl_str_get_lines(kcl_str* str, kcl_arena* arena);

#endif /* ndef KCL_LISTS */

