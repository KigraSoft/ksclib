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
};

struct kcl_list {
	struct kcl_arena *arena;
	struct kcl_lst_obj *list;
	struct kcl_lst_obj *current;
	enum kcl_lst_type type;
	uint count;
	uint size;
};

static struct kcl_list *
kcl_lst_alloc_list(enum kcl_lst_type type, struct kcl_arena *arena, uint num_elements)
{
	struct kcl_list *new_list = kcl_arn_push(arena, sizeof *new_list);
	if (!new_list) { return (false); }
	new_list->type = type;
	new_list->arena = arena;
	switch (type) {
	case LNKLST:
		new_list->list = NULL;
		new_list->current = NULL;
		new_list->count = 0;
		new_list->size = num_elements;
		return (new_list);
	case VARRAY:
		new_list->list = NULL;
		for (uint i = 0; i < num_elements; i++) {
			struct kcl_lst_obj *new_obj = kcl_arn_push(new_list->arena, sizeof *new_obj);
			new_obj->datum = NULL;
			new_obj->next = new_list->list;
			new_list->list = new_obj;
		}
		new_list->count = 0;
		new_list->size = num_elements;
		return (new_list);
	default:
		return (false);
	}
}

static uint
kcl_lst_add_datum(struct kcl_list *list, void *datum)
{
	switch (list->type) {
	case LNKLST:
		struct kcl_lst_obj *new_obj = kcl_arn_push(list->arena, sizeof *new_obj);
		new_obj->datum = datum;
		new_obj->next = list->list;
		list->list = new_obj;
		list->count++;
		return (list->count);
	default:
		return (0);
	}
}

static bool
kcl_lst_set_element(struct kcl_list *list, void *datum, uint index)
{
	switch (list->type) {
	case VARRAY:
		if (index >= list->size) {
			return (false);
		}
		struct kcl_lst_obj *obj = list->list;
		uint i = 0;
		while (i < index) {
			obj = obj->next;
			i++;
		}
		obj->datum = datum;
		return (true);
	default:
		return (false);
	}
}

static void *
kcl_lst_get_element(struct kcl_list *list, uint index)
{
	switch (list->type) {
	case VARRAY:
		if (index >= list->size) { return (NULL); }
		struct kcl_lst_obj *obj = list->list;
		uint i = 0;
		while (i < index) {
			obj = obj->next;
			i++;
		}
		return (obj->datum);
	default:
		return (NULL);
	}
}
			
static void *
kcl_lst_get_first(struct kcl_list *list)
{
	if (list->count > 0) {
		list->current = list->list;
		return (list->current->datum);
	} else {
		return (NULL);
	}
}

static void *
kcl_lst_get_next(struct kcl_list *list)
{
	if (list->current->next) {
		list->current = list->current->next;
		return (list->current->datum);
	} else {
		return (NULL);
	}
}

/*
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
*/	
