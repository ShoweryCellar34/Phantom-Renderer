#include <stdint.h>

extern void* prMalloc(size_t size);
extern void* prRealloc(void* sourceMemory, size_t size);
extern void* prMemmove(void* destinationMemory, const void* sourceMemory, size_t size);
extern void prFree(void* memory);

#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC prMalloc
#define STBI_REALLOC prRealloc
#define STBI_FREE prFree
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_MALLOC prMalloc
#define STBIW_REALLOC prRealloc
#define STBIW_MEMMOVE prMemmove
#define STBIW_FREE prFree
#include <stb_image_write.h>
