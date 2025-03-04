#include <PR/memory.h>

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <PR/error.h>

void* prMalloc(size_t size) {
    void* memory = malloc(size);
    char failMessage[50];
    snprintf(failMessage, 50, "Failed to allocate %llu bytes of memory", size);
    char successMessage[100];
    snprintf(successMessage, 50, "Succesfully allocated %llu bytes of memory", size);
    PR_MEMORY_CHECK(memory, failMessage, successMessage)
    return memory;
}

void* prMemcpy(void* destinatonMemory, void* sourceMemory, size_t size) {
    void* memory = memcpy(destinatonMemory, sourceMemory, size);
    char failMessage[50];
    snprintf(failMessage, 50, "Failed to copy %llu bytes of memory", size);
    char successMessage[100];
    snprintf(successMessage, 50, "Succesfully copied %llu bytes of memory", size);
    PR_MEMORY_CHECK(memory, failMessage, successMessage)
    return memory;
}

void prFree(void* memory) {
    free(memory);
}
