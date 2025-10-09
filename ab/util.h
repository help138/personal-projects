#ifndef MUTIL_H
#define MUTIL_H 1

#include <stdint.h>
#include <stdbool.h>
typedef uint64_t u64;
typedef uint8_t byte;
typedef uint32_t u32;
typedef int32_t i32;
typedef int64_t i64;
typedef int8_t i8;


#if defined(_STDLIB_H) && defined(DEBUG_MODE)

	FILE* LOG_STREAM = NULL;
#	define SET_LOG_STREAM(S) LOG_STREAM = S;
#	define WRITE_LOG(S){ int i = 0; while(S[i]){ putc(S[i], LOG_STREAM); i++; } }
#	define malloc(m) malloc(m); if(!LOG_STREAM) LOG_STREAM = stderr; fprintf(LOG_STREAM, "Allocating %llu byte\t\tline %d, file %s\n", m, __LINE__, __FILE__);
#	define calloc(m, s) calloc(m, s); if(!LOG_STREAM) LOG_STREAM = stderr; fprintf(LOG_STREAM, "Allocating %llu byte\t\tline %d, file %s\n", m*s, __LINE__, __FILE__);
#	define realloc(p, m) realloc(p, m); if(!LOG_STREAM) LOG_STREAM = stderr; fprintf(LOG_STREAM, "ReAllocating %llu byte at address %p\t\tline %d, file %s\n", m, p, __LINE__, __FILE__);
#	define free(p) free(p); if(!LOG_STREAM) LOG_STREAM = stderr; fprintf(LOG_STREAM, "Freeing address %p\t\tline %d, file %s\n", p, __LINE__, __FILE__);

#endif //allocation tracking

#if defined(STACK_TYPE)
#define MAX_STACK_SIZE 1024
typedef STACK_TYPE stack_t;

typedef struct{
	stack_t mem[MAX_STACK_SIZE];
	int top;
} stack;

#define IS_EMPTY(s) (s.top == -1)
#define IS_FULL(s) (s.top > MAX_STACK_SIZE)
#define PEEK(s) (s.top == -1) ? NULL: s.mem[s.top]
#define POP(s) (s.top == -1) ? NULL: s.mem[s.top--]
#define INIT_STACK(s) s = (stack){ .mem = {0}, .top = -1 }
#define PUSH(s, m) s.top++; if(s.top < MAX_STACK_SIZE) s.mem[s.top] = m

#define IS_EMPTY_PTR(s) (s->top == -1)
#define IS_FULL_PTR(s) (s->top > MAX_STACK_SIZE)
#define POP_PTR(s) (s->top == -1) ? NULL: s->mem[s->top--]
#define PUSH_PTR(s, m) s->top++; if(s->top < MAX_STACK_SIZE) s->mem[s->top] = m

#endif // stack stuff
#endif // header guard
