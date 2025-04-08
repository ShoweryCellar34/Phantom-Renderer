#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct prMeshData prMeshData;

typedef struct prDrawList {
    prMeshData** meshes;
    size_t meshesSize;
    size_t meshCount;
    bool isOrdered;
} prDrawList;

prDrawList* prDrawListCreate();

void prDrawListDestroy(prDrawList* drawList);

void prDrawListPush(prDrawList* drawList, prMeshData* mesh);

void prDrawListPushMultiple(prDrawList* drawList, prMeshData** meshes, size_t meshCount);

void prDrawListPop(prDrawList* drawlist);

void prDrawListPopMultiple(prDrawList* drawList, size_t meshCount);

void prDrawListPopAt(prDrawList* drawList, size_t index);

void prDrawListPopAll(prDrawList* drawList);

void prDrawListOrder(prDrawList* drawList);
