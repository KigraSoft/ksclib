/*
  kcl-test--memory.c
*/

#include <assert.h>
#include "../ksclib.c"

const unsigned int mblock_sz = 16;

int
main()
{
	struct kcl_arena *arena = kcl_arn_alloc(STACK, mblock_sz, mblock_sz, true);
	unsigned int *a = kcl_arn_push(arena, sizeof *a);
	*a = 1;
	printf("Line: %i: Val a: %i\n", __LINE__, *a);
	assert( *a == 1);
	*a = 2;
	printf("Line: %i: Val a: %i\n", __LINE__, *a);
	assert( *a == 2);
	printf("Size: %i\n", arena->size);
	unsigned int (*b)[20] = kcl_arn_push(arena, sizeof *b);
	*b[0] = 20;
	printf("Line: %i: Val b[0]: %i\n", __LINE__, *b[0]);
	*b[4] = 40;
	printf("Line: %i: Val b[4]: %i\n", __LINE__, *b[4]);
	printf("Size: %i\n", arena->size);
}
