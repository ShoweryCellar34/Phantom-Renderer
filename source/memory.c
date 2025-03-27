#include <PR/memory.h>

#include <PR/defines.h>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <PR/error.h>

void* prMalloc(size_t size) {
    void* memory = malloc(size);

    char failMessage[96];
    snprintf(failMessage, 96, "Failed to allocate %zu bytes of memory", size);
    char successMessage[96];
    snprintf(successMessage, 96, "Succesfully allocated %zu bytes of memory", size);
    if(!memory) {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_FTAL, failMessage);
        return NULL;
    } else {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_TRCE, successMessage);
    }

    return memory;
}

void* prMemcpy(void* destinatonMemory, void* sourceMemory, size_t size) {
    void* memory = memcpy(destinatonMemory, sourceMemory, size);

    char failMessage[96];
    snprintf(failMessage, 96, "Failed to copy %zu bytes of memory", size);
    char successMessage[96];
    snprintf(successMessage, 96, "Succesfully copied %zu bytes of memory", size);
    if(!memory) {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_FTAL, failMessage);
        return NULL;
    } else {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_TRCE, successMessage);
    }

    return memory;
}

void* prRealloc(void* sourceMemory, size_t size) {
    void* memory = realloc(sourceMemory, size);

    char failMessage[96];
    snprintf(failMessage, 96, "Failed to reallocate %zu bytes of memory", size);
    char successMessage[96];
    snprintf(successMessage, 96, "Succesfully reallocated %zu bytes of memory", size);
    if(!memory) {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_FTAL, failMessage);
        return NULL;
    } else {
        prLogEvent(PR_MMRY_EVENT, PR_LOG_TRCE, successMessage);
    }

    return memory;
}

void prFree(void* memory) {
    free(memory);

    prLogEvent(PR_MMRY_EVENT, PR_LOG_TRCE, "Successfully deallocated memory");
}
