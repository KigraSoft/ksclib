/*
  kcl-test--string.c
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

	kcl_str* str1 = kcl_str_new(one, 2, arena);
	printf("Test 1: %s\n", kcl_str_cstr_alloc(str1));

	kcl_str* str23 = kcl_str_concat_new(two, 3, three, 5, arena);
	printf("Test 2: %s\n", kcl_str_cstr_alloc(str23));	

	kcl_str* str3 = kcl_str_slice_new(str23, 0, 5, arena);
	printf("Test 3: %s\n", kcl_str_cstr_alloc(str3));	

	return EXIT_SUCCESS;
}
