/*
  ksclib-lists.c

  The KCL Lists module provides an implementation of various dynamic
  list types using the KCL Arena library for memory.

  Standard function prefix:  kcl_lst_

*/

enum kcl_lst_type {
	VARRAY,
	LNKLST
};

struct kcl_lst_obj {
	void *datum;
	struct kcl_lst_obj *next;
	uint key;
};

struct kcl_list {
	struct kcl_arena *arena;
	struct kcl_lst_obj *list;
	enum kcl_lst_type type;
	uint count;
};

static struct kcl_list *
kcl_lst_alloc_list(enum kcl_lst_type type, struct kcl_arena *arena)
{
	struct kcl_list *new_list = kcl_arn_push(arena, sizeof *new_list);
	if (!new_list) { return (false); }
	new_list->type = type;
	new_list->arena = arena;
	new_list->list = NULL;
	new_list->count = 0;
	return (new_list);
}

static uint
kcl_lst_add_datum(struct kcl_list *list, void *datum, uint key)
{
	struct kcl_lst_obj *new_obj = kcl_arn_push(list->arena, sizeof *new_obj);
	new_obj->key = key;
	new_obj->datum = datum;
	new_obj->next = list->list;
	list->list = new_obj;
	list->count++;
	return (list->count);
}

static void *
kcl_lst_get_datum(struct kcl_list *list, uint key)
{
	struct kcl_lst_obj *obj = list->list;
	while (obj != NULL) {
		if (obj->key == key) {
			return (obj->datum);
		} else {
			obj = obj->next;
		}
	}
	return (NULL);
}
	
