/*
  kcl-test--lists.c
*/

#include <assert.h>
#include "../ksclib.c"

const uint mblock_sz = 16;

int
main()
{
	//printf("File/Line: %s/%i:\n", __FILE__, __LINE__);
	struct kcl_arena *arena = kcl_arn_alloc(STACK, mblock_sz, mblock_sz, true);
	struct kcl_list *list = kcl_lst_alloc_list(LNKLST, arena);

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
}
