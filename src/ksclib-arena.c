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

static struct kcl_arena *
kcl_arn_alloc(enum kcl_arn_type type, size_t arena_size, size_t increment, bool autogrow)
{
	struct kcl_arena *new_arena = malloc(sizeof(struct kcl_arena));
	if (!new_arena) { return 0; }

	new_arena->memblock = malloc(arena_size);
	if (!new_arena) { return 0; }

	new_arena->size = arena_size;
	new_arena->stack_pos = 0;
	new_arena->type = type;
	new_arena->inc_size = increment;
	new_arena->autogrow = autogrow;
	return (new_arena);
}

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

static void
kcl_arn_reset(struct kcl_arena *arena)
{
	arena->stack_pos = 0;
}
