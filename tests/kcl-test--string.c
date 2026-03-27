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
	//char *tmp;
	
	//struct kcl_arena *arena = nullptr;
	//kcl_arn_alloc(&arena, STACK, mblock_sz, mblock_sz, true);
	kcl_arena *arena = kcl_arn_alloc(STACK, mblock_sz, mblock_sz, true);

	kcl_str* str1 = kcl_str_new(one, 2, arena);
	printf("Test 1: %s\n", kcl_str_cstr_alloc(str1));

	kcl_str* str23 = kcl_str_concat_new(two, 3, three, 5, arena);
	printf("Test 2: %s\n", kcl_str_cstr_alloc(str23));	

	kcl_str* str3 = kcl_str_slice_new(str23, 0, 5, arena);
	printf("Test 3: %s\n", kcl_str_cstr_alloc(str3));	

	kcl_str_set_value(str23, "four");
	printf("Test 4: %s\n", kcl_str_cstr_alloc(str23));

	kcl_str_slice(str1, str23, 1, 2);
	printf("Test 5: %s\n", kcl_str_cstr_alloc(str1));
	
	kcl_str_set_concat(str23, three, 1, two, 1);
	printf("Test 6: %s\n", kcl_str_cstr_alloc(str23));

	kcl_str* str4a = kcl_str_new("Hello", 12, arena);
	kcl_str* str4b = kcl_str_new("world!", 8, arena);
	printf("Test 7a: %s - %u - %u\n", kcl_str_cstr_alloc(str4a), str4a->len, str4a->size);
	printf("Test 7b: %s\n", kcl_str_cstr_alloc(str4b));
	kcl_str_append(str4a, " ");
	printf("Test 7c: %s\n", kcl_str_cstr_alloc(str4a));
	kcl_str_append(str4a, str4b);
	printf("Test 7d: %s\n", kcl_str_cstr_alloc(str4a));
	printf("Test 7d: %s\n", kcl_str_to_cstr_new(str4a, arena));
	printf("Test 7d: ");
	kcl_str_fputs(str4a, stdout);
	printf("\n");

	kcl_str* str5a = kcl_str_new("To be ", 20, arena);
	kcl_str* str5b = kcl_str_new("or ", 4, arena);
	kcl_str_append(str5a, str5b);
	kcl_str_append(str5a, "not to be.\n");
	printf("Test 8: ");
	kcl_str_fputs(str5a, stdout);
	printf("\n");
	
	return EXIT_SUCCESS;
}
