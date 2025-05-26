#include <PR/memory.h>

#include <PR/defines.h>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <PR/logger.h>

void* prMalloc(size_t size) {
    prLogEvent(PR_EVENT_MEMORY, PR_LOG_TRACE, "prMalloc: Allocating %zu bytes of memory", size);

    void* memory = malloc(size);

    if(!memory) {
        prLogEvent(PR_EVENT_MEMORY, PR_LOG_FATAL, "prMalloc: Failed to allocate %zu bytes of memory", size);
        return NULL;
    }

    return memory;
}

void* prCalloc(size_t objects, size_t size) {
    prLogEvent(PR_EVENT_MEMORY, PR_LOG_TRACE, "prCalloc: Allocating %zu objects of size %zu bytes of memory and initializing to 0", objects, size);

    void* memory = calloc(objects, size);

    if(!memory) {
        prLogEvent(PR_EVENT_MEMORY, PR_LOG_FATAL, "prCalloc: Failed to allocate %zu objects of size %zu bytes of memory and initializing to 0", objects, size);
        return NULL;
    }

    return memory;
}

void* prRealloc(void* sourceMemory, size_t size) {
    prLogEvent(PR_EVENT_MEMORY, PR_LOG_TRACE, "prRealloc: Reallocating %zu bytes of memory", size);

    void* memory = realloc(sourceMemory, size);

    if(!memory) {
        prLogEvent(PR_EVENT_MEMORY, PR_LOG_FATAL, "prRealloc: Failed to reallocate %zu bytes of memory", size);
        return NULL;
    }

    return memory;
}

void* prMemcpy(void* destinatonMemory, void* sourceMemory, size_t size) {
    prLogEvent(PR_EVENT_MEMORY, PR_LOG_TRACE, "prMemcpy: Copying %zu bytes of memory", size);

    void* memory = memcpy(destinatonMemory, sourceMemory, size);

    if(!memory) {
        prLogEvent(PR_EVENT_MEMORY, PR_LOG_FATAL, "prMemcpy: Failed to copy %zu bytes of memory", size);
        return NULL;
    }

    return memory;
}

void prFree(void* memory) {
    prLogEvent(PR_EVENT_MEMORY, PR_LOG_TRACE, "prFree: Deallocating memory");

    free(memory);
}
