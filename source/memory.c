#include <PR/memory.h>

#include <PR/defines.h>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <PR/error.h>

void* prMalloc(size_t size) {
    void* memory = malloc(size);

    if(!memory) {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_FTAL, "Failed to allocate %zu bytes of memory", size);
        return NULL;
    } else {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_TRCE, "Succesfully allocated %zu bytes of memory", size);
    }

    return memory;
}

void* prMemcpy(void* destinatonMemory, void* sourceMemory, size_t size) {
    void* memory = memcpy(destinatonMemory, sourceMemory, size);

    if(!memory) {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_FTAL, "Failed to copy %zu bytes of memory", size);
        return NULL;
    } else {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_TRCE, "Succesfully copied %zu bytes of memory", size);
    }

    return memory;
}

void* prRealloc(void* sourceMemory, size_t size) {
    void* memory = realloc(sourceMemory, size);

    if(!memory) {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_FTAL, "Failed to reallocate %zu bytes of memory", size);
        return NULL;
    } else {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_TRCE, "Succesfully reallocated %zu bytes of memory", size);
    }

    return memory;
}

void prFree(void* memory) {
    free(memory);

    prLogEvent(PR_MMRY_EVENT, PR_LOG_TRCE, "Successfully deallocated memory");
}
