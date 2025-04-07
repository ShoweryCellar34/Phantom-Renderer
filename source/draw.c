#include <PR/draw.h>

#include <PR/memory.h>
#include <PR/mesh.h>

prDrawList* prDrawListCreate() {
    prDrawList* drawList = prCalloc(0, sizeof(prDrawList));

    return drawList;
}

void prDrawListDestroy(prDrawList* drawList) {
    prFree(drawList);
}

void prDrawListPush(prDrawList* drawList, prMeshData* mesh) {
    prMeshData* temp = prMalloc(drawList->meshCount + 1);
    prMemcpy(temp, drawList->meshes, drawList->meshCount);
    temp[drawList->meshCount] = mesh;

    if(drawList->meshes) {
        prFree(drawList->meshes);
    }

    drawList->meshes = temp;
    drawList->meshCount++;
}

void prDrawListPop(prDrawList* drawList) {
    prMeshData* temp = prMalloc(drawList->meshCount - 1);
    prMemcpy(temp, drawList->meshes, drawList->meshCount - 1);

    if(drawList->meshes) {
        prFree(drawList->meshes);
    }

    drawList->meshes = temp;
    drawList->meshCount--;
}
