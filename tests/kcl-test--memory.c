/*
  kcl-test--memory.c
*/

#include <assert.h>
#include "../ksclib.c"
#include "../src/ksclib-debug.c"

// #define printint(s,x) do { printf("File/Line: %s/%i\t%s: %u - %p\n", __FILE__, __LINE__, s, x, &x); } while (0)

// #define arena_on_stack

const unsigned int mblock_sz = 256;

int
main()
{
#ifdef arena_on_stack
	printf(">+> on stack\n");
	kcl_arena arena_stack;
	kcl_arena *arena = &arena_stack;
	kcl_arn_alloc(&arena, STACKPLUS, 64, mblock_sz, true);
#else
	printf(">+> on heap\n");
	kcl_arena *arena = nullptr;
	kcl_arn_alloc(&arena, STACKPLUS, 64, mblock_sz, true);
#endif
	printf("arena:\t\t%p\n", arena);
	//struct kcl_arena *arena = kcl_arn_alloc(nullptr, STACKPLUS, 64, mblock_sz, true);
	kcl_arn_mem_display(arena, (uintptr_t)arena, 96);
	unsigned int *a = kcl_arn_push(arena, sizeof *a);
	*a = 1;
	kcl_dbg_printvar("*a", a);
	assert( *a == 1);
	kcl_arn_mem_display(arena, (uintptr_t)a, 96);

	*a = 2;
	kcl_dbg_printvar("*a", a);
	assert( *a == 2);

	//unsigned int (*b)[20] = kcl_arn_push(arena, sizeof *b);
	unsigned int *b = kcl_arn_push(arena, 20 * (sizeof *b));
	kcl_arn_mem_display(arena, (uintptr_t)b, 96);

	b[0] = 255;
	b[1] = (256 * 256) - 1;
	kcl_dbg_printvar("b[0]", b);
	kcl_dbg_printvar("b[1]", b + 1);

	kcl_arn_mem_display(arena, (uintptr_t)b, 128);

	//struct kcl_arena *arena2 = kcl_arn_alloc(STACK, 128, 128, true);
	kcl_arn_reset(arena);
	kcl_arn_mem_display(arena, (uintptr_t)a, 96);

	char* abc = kcl_arn_push(arena, 30);
	abc[0] = 0;
	//kcl_arn_mem_display(arena, (uintptr_t)a, 96);
	kcl_dbg_printvar("abc", abc);
	strcpy(abc, "abcdefghijklmnopqrstuvwxyz");
	kcl_dbg_printvar("abc", abc);
	kcl_arn_mem_display(arena, (uintptr_t)abc, 96);

	exit(EXIT_SUCCESS);
}
