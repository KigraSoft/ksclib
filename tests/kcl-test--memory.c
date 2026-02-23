/*
  kcl-test--memory.c
*/

#include <assert.h>
#include "../ksclib.c"

const unsigned int mblock_sz = 160;

int
main()
{
	struct kcl_arena *arena = kcl_arn_alloc(STACK, mblock_sz, mblock_sz, true);
	unsigned int *a = kcl_arn_push(arena, sizeof *a);
	*a = 1;
	printf("Line: %i: Val a: %i - %p\n", __LINE__, *a, a);
	assert( *a == 1);
	kcl_arn_mem_display(arena, 96);
	*a = 2;
	printf("Line: %i: Val a: %i\n", __LINE__, *a);
	assert( *a == 2);
	printf("Size: %li\n", arena->size);
	unsigned int (*b)[20] = kcl_arn_push(arena, sizeof *b);
	*b[0] = 20;
	printf("Line: %i: Val b[0]: %i\n", __LINE__, *b[0]);
	*b[4] = 40;
	printf("Line: %i: Val b[4]: %i\n", __LINE__, *b[4]);
	printf("Size: %li\n\n", arena->size);

	printf("Line: %i: Val a: %i - %p\n", __LINE__, *a, a);
	kcl_arn_mem_display(arena, 96);

	//struct kcl_arena *arena2 = kcl_arn_alloc(STACK, 128, 128, true);
	kcl_arn_reset(arena);
	char* abc = kcl_arn_push(arena, 30);
	strcpy(abc, "abcdefghijklmnopqrstuvwxyz");
	//printf("%p\n%p\n%p\n", arena, arena->memblock, abc);
	printf("\nTest str: %s - %p\n\n", abc, abc);
	kcl_arn_mem_display(arena, 96);
}
