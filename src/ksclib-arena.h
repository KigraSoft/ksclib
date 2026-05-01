/*
  ksclib-arena.h

  The KCL Arena module provides an implementation of a memory arena.

  Standard function prefix:  kcl_arn_

*/

#ifndef KCL_ARENA
#define KCL_ARENA

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

kcl_arena * kcl_arn_alloc(enum kcl_arn_type type, size_t arena_size, size_t increment, bool autogrow);
void kcl_arn__mem_display_line(void* line_mem, size_t base_address, unsigned int line_len);
void kcl_arn_mem_display(struct kcl_arena *arena, uintptr_t disp_address, size_t display_vals);
static bool kcl_arn_grow(struct kcl_arena *arena, uintptr_t req_size);
static void * kcl_arn_push(struct kcl_arena *arena, size_t size);
static void kcl_arn_reset(struct kcl_arena *arena);
static void kcl_arn_free(kcl_arena* arena);

#endif /* ndef KCL_ARENA */

