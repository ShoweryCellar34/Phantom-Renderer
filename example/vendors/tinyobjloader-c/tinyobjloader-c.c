#include <stdint.h>

extern void* prMalloc(size_t size);
extern void* prRealloc(void* sourceMemory, size_t size);
extern void* prCalloc(size_t objects, size_t size);
extern void prFree(void* memory);

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#define TINYOBJ_MALLOC prMalloc
#define TINYOBJ_REALLOC prRealloc
#define TINYOBJ_CALLOC prCalloc
#define TINYOBJ_FREE prFree
#include <tinyobj_loader_c.h>
