/*
  ksclib-arena.c

  The KCL Arena module provides an implementation of a memory arena.

  Standard function prefix:  kcl_arn_

*/

enum kcl_arn_type {
	STACK
};

struct kcl_arena {
	void        *memblock;
	uintptr_t    stack_pos;
	size_t       size;
	size_t       inc_size;
	enum kcl_arn_type type;
	bool         autogrow;
};

/*
struct kcl_arena *
kcl_arn_init(void *new_memblock, size_t memblock_size)
{
	struct kcl_arena *new_arena = new_memblock;
	new_arena->size = memblock_size;
	new_arena->stack_pos = sizeof *new_arena;
	new_arena->type = STACK;
	return (new_arean);
}
*/

[[maybe_unused]]
static struct kcl_arena *
kcl_arn_alloc(enum kcl_arn_type type, size_t arena_size, size_t increment, bool autogrow)
{
	struct kcl_arena *new_arena = malloc(sizeof(struct kcl_arena));
	if (!new_arena) { return 0; }

	new_arena->memblock = malloc(arena_size);
	if (!new_arena->memblock) { return 0; }

	new_arena->size = arena_size;
	new_arena->stack_pos = 0;
	new_arena->type = type;
	new_arena->inc_size = increment;
	new_arena->autogrow = autogrow;
	return (new_arena);
}

[[maybe_unused]]
void
kcl_arn__mem_display_line(void* line_mem, size_t base_address, unsigned int line_len)
{
	const unsigned int max_line_len = 16;
	line_len = line_len <= max_line_len ? line_len : max_line_len;
	unsigned char* mem_vals = (unsigned char*)line_mem;
	size_t mem_line_address = (uintptr_t)line_mem;
	
	printf("%04lx: ", mem_line_address - base_address);
	
	for (unsigned int i = 0; i < line_len; i++) {
		if (!(i % 8)) {
			printf("  ");
		}
		printf("%02X ", mem_vals[i]);
	}
	for (unsigned int i = 0; i < line_len; i++) {
		if (!(i % 8)) {
			printf("  ");
		}
		if (mem_vals[i] < 32 || mem_vals[i] > 126) {
			printf(".");
		} else {
			printf("%c", mem_vals[i]);
		}
	}
	printf("\n");
}
		
[[maybe_unused]]
void
kcl_arn_mem_display(struct kcl_arena *arena, size_t display_vals)
{
	size_t num_display_vals = display_vals <= arena->size ? display_vals : arena->size;
	unsigned int num_line_vals = 16;
	printf("Arena Memory - Type: %s\n", "STACK");
	printf("  Address: %p\tSize: %lu\tInc Size: %lu\n\n", arena->memblock, arena->size, arena->inc_size);
	size_t base_mem_address = (uintptr_t)&arena->memblock >> 16 << 16;
	size_t i = 0;
	unsigned int j = 0;
	void* cur_line_mem = arena->memblock;
	while (i < num_display_vals) {
		j = (i + num_line_vals) < num_display_vals ? num_line_vals : num_display_vals;
		kcl_arn__mem_display_line(cur_line_mem, base_mem_address, j);
		i += num_line_vals;
		cur_line_mem += num_line_vals;
	}
}

[[maybe_unused]]
static bool
kcl_arn_grow(struct kcl_arena *arena, uintptr_t req_size)
{
	size_t new_size = arena->size;
	while (new_size < req_size) {
		new_size += arena->inc_size;
	}
	if (new_size >= req_size) {
		struct kcl_arena *new_arena = realloc(arena->memblock, new_size);
		if (new_arena) {
			arena->memblock = new_arena;
			arena->size = new_size;
			return (true);
		} else { return (false); }
	} else { return (true); }
}

[[maybe_unused]]
static void *
kcl_arn_push(struct kcl_arena *arena, size_t size)
{
	// currently assumes type == STACK; when new type added need switch statement, etc.
	uintptr_t cur_pos = arena->stack_pos;
	uintptr_t new_pos = cur_pos + size;

	while ((new_pos % _Alignof(max_align_t)) != 0 ) {
		new_pos++;
	}

	if (new_pos > arena->size) {
		// !!!!! this was not a good idea and current pointers could be invalid after realloc
		if (!arena->autogrow) {
			return (0);
		} else if (!kcl_arn_grow(arena, new_pos)) {
			return (0);
		}
	}

	void *new_ptr = arena->memblock + cur_pos;
	arena->stack_pos = new_pos;
	return (new_ptr);
}

[[maybe_unused]]
static void
kcl_arn_reset(struct kcl_arena *arena)
{
	arena->stack_pos = 0;
}
