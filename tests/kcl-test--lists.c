/*
  kcl-test--lists.c
*/

#include <assert.h>
#include "../ksclib.c"

const unsigned int mblock_sz = 1024;

int
main()
{
	const char *one = "one";
	const char *two = "two";
	const char *three = "three";
	char *tmp;

	struct kcl_arena *arena = kcl_arn_alloc(STACK, mblock_sz, mblock_sz, true);
	struct kcl_list *list = kcl_lst_alloc_list(LNKLST, arena, 0);

	kcl_lst_add_datum(list, (void *)one);
	kcl_lst_add_datum(list, (void *)two);
	kcl_lst_add_datum(list, (void *)three);

	tmp = (char *)kcl_lst_get_first(list);
	while (tmp) {
		printf("File/Line: %s/%i: Str: %s\n", __FILE__, __LINE__, tmp);
		tmp = (char *)kcl_lst_get_next(list);
	}

	kcl_lst_del_datum(list, (void *)two);
	tmp = (char *)kcl_lst_get_first(list);
	while (tmp) {
		printf("File/Line: %s/%i: Str: %s\n", __FILE__, __LINE__, tmp);
		tmp = (char *)kcl_lst_get_next(list);
	}

	struct kcl_list *varray = kcl_lst_alloc_list(VARRAY, arena, 10);
	kcl_lst_set_element(varray, (void *)two, 0);
	tmp = (char *)kcl_lst_get_element(varray, 0);
	printf("File/Line: %s/%i: Idx: %i, Str: %s\n", __FILE__, __LINE__, 0, tmp);

	/*
	uint *a = kcl_arn_push(arena, sizeof *a);
	*a = 11;
	kcl_lst_add_datum(list, a, 1);

	uint *b = kcl_arn_push(arena, sizeof *a);
	*b = 22;
	kcl_lst_add_datum(list, b, 2);

	uint *c = kcl_lst_get_datum(list, 1);
	printf("Line: %i: Val a: %i\n", __LINE__, *a);
	printf("Line: %i: Val b: %i\n", __LINE__, *b);
	printf("Line: %i: Val c: %i\n", __LINE__, *c);
	*/
	
	/*
	assert( *a == 1);
	*a = 2;
	printf("Line: %i: Val a: %i\n", __LINE__, *a);
	assert( *a == 2);
	printf("Size: %i\n", arena->size);
	uint (*b)[20] = kcl_arn_push(arena, sizeof *b);
	*b[0] = 20;
	printf("Line: %i: Val b[0]: %i\n", __LINE__, *b[0]);
	*b[4] = 40;
	printf("Line: %i: Val b[4]: %i\n", __LINE__, *b[4]);
	printf("Size: %i\n", arena->size);
	*/

	printf(">>> testing...\n");
	struct kcl_list *kvlist = kcl_lst_alloc_list(KV_STR, arena, 0);
	kcl_str* str_one = kcl_str_new("ONE", 3, arena);
	kcl_str* str_two = kcl_str_new("TWO", 3, arena);
	kcl_lst_add_datum_w_key(kvlist, (void *)one, str_one);
	printf(">>> add datum complete - %u\n", kvlist->count);
	tmp = (char *)kcl_lst_get_val(kvlist, str_two);
	printf(">>> val: %s", tmp);
}
