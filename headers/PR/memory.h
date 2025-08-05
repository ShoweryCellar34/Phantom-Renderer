#pragma once

#include <stddef.h>

void* prMalloc(size_t size);

void* prCalloc(size_t objects, size_t size);

void* prRealloc(void* sourceMemory, size_t size);

void* prMemcpy(void* destinatonMemory, const void* sourceMemory, size_t size);

void* prMemmove(void* destinationMemory, const void* sourceMemory, size_t size);

void prFree(void* memory);
