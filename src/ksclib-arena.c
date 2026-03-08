/*
  ksclib-arena.c

  The KCL Arena module provides an implementation of a memory arena.

  Standard function prefix:  kcl_arn_

*/

/**
  \file ksclib-arena.c
  \brief KigraSoft C Library:  Arena

  A memory arena implementation

*/

enum kcl_arn_type {
	STACK,
	STACKPLUS,
};

typedef struct kcl_arn__memblock {
	void                 *memblock;
	uintptr_t             stack_pos;
	size_t                size;
	struct kcl_arn__memblock    *next;
} kcl_arn__memblock;

typedef struct kcl_arena {
	size_t       size;
	size_t       inc_size;
	enum kcl_arn_type type;
	bool         autogrow;
	kcl_arn__memblock *memblocks;
	kcl_arn__memblock *memblock_cur;
	unsigned int memblocks_num;
} kcl_arena;

/// kcl_arn_alloc brief desc
/**
   Document test for kcl_arn_alloc.
*/
[[maybe_unused]]
bool
kcl_arn_alloc(kcl_arena **arena_ptr, enum kcl_arn_type type, size_t arena_size, size_t increment, bool autogrow)
{
	if (!(*arena_ptr)) {
		*arena_ptr = malloc(sizeof(kcl_arena));
	}
	kcl_arena *arena = *arena_ptr;
	if (!arena) { return false; }
	arena->memblock_cur = malloc(arena_size);
	if (!arena->memblock_cur) { return (false); }
	arena->memblock_cur->memblock = arena->memblock_cur;
	arena->memblock_cur->next = nullptr;
	arena->memblocks = arena->memblock_cur;
	arena->size = arena_size;
	arena->memblocks_num = 1;
	arena->memblock_cur->size = arena_size;
	arena->memblock_cur->stack_pos = 0 + sizeof (kcl_arn__memblock);
	arena->type = type;

	switch (type) {
	case STACKPLUS:
		arena->inc_size = increment;
		arena->autogrow = autogrow;
		break;
	case STACK:
		arena->inc_size = 0;
		arena->autogrow = false;
		break;
	}
	return (true);

}

/// kcl_arn__mem_display_line brief desc
/**
   Document test for kcl_arn__mem_display_line.
*/
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
		
/// kcl_arn_mem_display brief desc
/**
   Document test for kcl_arn_mem_display.
*/
[[maybe_unused]]
void
kcl_arn_mem_display(struct kcl_arena *arena, uintptr_t disp_address, size_t display_vals)
{
	size_t num_display_vals = display_vals <= arena->size ? display_vals : arena->size;
	unsigned int num_line_vals = 16;
	switch (arena->type) {
	case STACK:
		printf("Arena Memory - Type: %s\n", "STACK");
		printf("  Address: %p\tSize: %lu\tCur Stack Pos: %lu\n\n", arena->memblock_cur, arena->size, arena->memblock_cur->stack_pos);
		break;
	case STACKPLUS:
		printf("Arena Memory - Type: %s\n", "STACKPLUS");
		printf("  Address: %p\tSize: %lu\tInc Size: %lu\n", arena->memblocks, arena->size, arena->inc_size);
		printf("  Num Blocks: %u\t\tCurrent block:\n", arena->memblocks_num);
		printf("  Address: %p\tSize: %lu\tCur Stack Pos: %lu / %lx\n\n", arena->memblock_cur, arena->memblock_cur->size, arena->memblock_cur->stack_pos, arena->memblock_cur->stack_pos);
		break;
	}
	size_t base_mem_address = (uintptr_t)&arena->memblock_cur >> 16 << 16;
	size_t i = 0;
	unsigned int j = 0;
	void* cur_line_mem = arena->memblock_cur;
	while (i < num_display_vals) {
		j = (i + num_line_vals) < num_display_vals ? num_line_vals : num_display_vals;
		kcl_arn__mem_display_line(cur_line_mem, base_mem_address, j);
		i += num_line_vals;
		cur_line_mem += num_line_vals;
	}
	printf("\n");
}

/// kcl_arn_grow brief desc
/**
   Document test for kcl_arn_grow.
*/
[[maybe_unused]]
static bool
kcl_arn_grow(struct kcl_arena *arena, uintptr_t req_size)
{
	switch (arena->type) {
	case STACK:
		/*
		  if code below is implemented, growing a basic STATIC invalidates all previous pushes
		  might be an option for some use cases
		size_t new_size = arena->size;
		while (new_size < req_size) {
			new_size += arena->inc_size;
		}
		if (new_size >= req_size) {
			struct kcl_arena *new_arena = realloc(arena->memblock_cur, new_size);
			if (new_arena) {
				arena->memblock_cur = new_arena;
				arena->size = new_size;
				return (true);
			} else { return (false); }
		} else { return (true); }
		*/
		return (false);
	case STACKPLUS:
		arena->memblock_cur->next = malloc(arena->inc_size);
		if (!arena->memblock_cur->next) { return (false); }
		arena->memblock_cur = arena->memblock_cur->next;
		arena->memblock_cur->memblock = arena->memblock_cur;
		arena->memblock_cur->stack_pos = 0 + sizeof (kcl_arn__memblock);
		arena->memblock_cur->size = arena->inc_size;
		arena->memblock_cur->next = nullptr;
		arena->size += arena->inc_size;
		arena->memblocks_num++;
		return (true);
	}
	return (false);
}

/// kcl_arn_push brief desc
/**
   Document test for kcl_arn_push.
*/
[[maybe_unused]]
static void *
kcl_arn_push(struct kcl_arena *arena, size_t size)
{
	uintptr_t new_ptr;
	uintptr_t cur_pos = arena->memblock_cur->stack_pos;
	uintptr_t new_pos = cur_pos + size;
	switch(arena->type) {
	case STACK:
		while ((new_pos % _Alignof(max_align_t)) != 0 ) {
			new_pos++;
		}
		if (new_pos > arena->memblock_cur->size) { return (0); }
		new_ptr = (uintptr_t)arena->memblock_cur + cur_pos;
		arena->memblock_cur->stack_pos = new_pos;
		return ((void *)new_ptr);
	case STACKPLUS:
		// doing this here could be wastefull of memory in an edge case
		while((new_pos % _Alignof(max_align_t)) != 0) {
			new_pos++;
		}

		if (new_pos > arena->memblock_cur->size) {
			if (!arena->autogrow) {
				return (nullptr);
			} else if (!kcl_arn_grow(arena, new_pos)) {
				return (nullptr);
			}
			cur_pos = arena->memblock_cur->stack_pos;
			new_pos = cur_pos + size;
			while((new_pos % _Alignof(max_align_t)) != 0) {
				new_pos++;
			}
		}

		new_ptr = (uintptr_t)arena->memblock_cur + cur_pos;
		arena->memblock_cur->stack_pos = new_pos;
		return ((void *)new_ptr);
	}
	return (nullptr);
}

[[maybe_unused]]
static void
kcl_arn_reset(struct kcl_arena *arena)
{
	kcl_arn__memblock *tmp;
	kcl_arn__memblock *tmp_prev;
	switch(arena->type) {
	case STACK:
		arena->memblock_cur->stack_pos = 0 + sizeof (kcl_arn__memblock);
		break;
	case STACKPLUS:
		while (arena->memblocks->next) {
			tmp_prev = arena->memblocks;
			tmp = arena->memblocks->next;
			while (tmp->next) {
				tmp_prev = tmp;
				tmp = tmp->next;
			}
			free(tmp);
			tmp_prev->next = nullptr;
			arena->size -= arena->inc_size;
			arena->memblock_cur = arena->memblocks;
			arena->memblocks_num--;
		}
		arena->memblock_cur->stack_pos = 0 + sizeof (kcl_arn__memblock);
		break;
	}
}

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

