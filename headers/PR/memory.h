#pragma once

#include <stddef.h>

void* prMalloc(size_t size);

void* prCalloc(size_t objects, size_t size);

void* prRealloc(void* sourceMemory, size_t size);

void* prMemcpy(void* destinatonMemory, void* sourceMemory, size_t size);

void prFree(void* memory);
