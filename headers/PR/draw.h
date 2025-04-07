#pragma once

#include <stddef.h>

typedef prMeshData prMeshData;

typedef struct prDrawList {
    prMeshData* meshes;
    size_t meshesSize;
    size_t meshCount;
} prDrawList;

prDrawList* prDrawListCreate();

void prDrawListDestroy(prDrawList* drawList);

void prDrawListPush(prDrawList* drawList, prMeshData* mesh);

void prDrawListPop(prDrawList* drawlist);
