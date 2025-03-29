#pragma once

#include <PR/defines.h>

void* prMalloc(memorySize_t size);

void* prMemcpy(void* destinatonMemory, void* sourceMemory, memorySize_t size);

void* prRealloc(void* sourceMemory, memorySize_t size);

void prFree(void* memory);
