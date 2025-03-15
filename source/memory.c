#include <PR/memory.h>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <PR/error.h>

void* prMalloc(size_t size) {
    void* memory = malloc(size);

    char failMessage[96];
    snprintf(failMessage, 96, "Failed to allocate %u bytes of memory", size);
    char successMessage[96];
    snprintf(successMessage, 96, "Succesfully allocated %u bytes of memory", size);
    if(!memory) {
        prError(PR_MEMORY_ERROR, failMessage);
        return NULL;
    } else {
        prLogTrace("[MEMORY]", successMessage);
    }

    return memory;
}

void* prMemcpy(void* destinatonMemory, void* sourceMemory, size_t size) {
    void* memory = memcpy(destinatonMemory, sourceMemory, size);

    char failMessage[96];
    snprintf(failMessage, 96, "Failed to copy %u bytes of memory", size);
    char successMessage[96];
    snprintf(successMessage, 96, "Succesfully copied %u bytes of memory", size);
    if(!memory) {
        prError(PR_MEMORY_ERROR, failMessage);
        return NULL;
    } else {
        prLogTrace("[MEMORY]", successMessage);
    }

    return memory;
}

void prFree(void* memory) {
    free(memory);

    prLogTrace("[MEMORY]", "Successfully deallocated memory");
}
