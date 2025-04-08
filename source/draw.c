#include <PR/draw.h>

#include <PR/defines.h>

#include <string.h>
#include <stdlib.h>
#include <PR/memory.h>
#include <PR/logger.h>
#include <PR/mesh.h>

void i_prDrawListResize(prDrawList* drawList, size_t desiredElements) {
    if(drawList->meshesSize >= desiredElements) {
        return;
    }

    if(drawList->meshesSize < desiredElements || drawList->meshesSize > (size_t)(desiredElements / PR_GROWTH_FACTOR)) {
        if(drawList->meshes) {
            drawList->meshes = prRealloc(drawList->meshes, (size_t)(desiredElements * PR_GROWTH_FACTOR) * sizeof(prMeshData*));
        } else {
            drawList->meshes = prMalloc((size_t)(desiredElements * PR_GROWTH_FACTOR) * sizeof(prMeshData*));
        }
        drawList->meshesSize = (size_t)(desiredElements * PR_GROWTH_FACTOR);
    }
}

prDrawList* prDrawListCreate() {
    prDrawList* drawList = prCalloc(1, sizeof(prDrawList));

    return drawList;
}

void prDrawListDestroy(prDrawList* drawList) {
    if(drawList->meshes) {
        prFree(drawList->meshes);
    }

    prFree(drawList);
}

void prDrawListPush(prDrawList* drawList, prMeshData* mesh) {
    i_prDrawListResize(drawList, drawList->meshCount + 1);
    drawList->meshes[drawList->meshCount] = mesh;

    drawList->meshCount++;
}

void prDrawlistPushMultiple(prDrawList* drawList, prMeshData** meshes, size_t meshCount) {
    if(meshCount == 0) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "No meshes to push to the draw list. Aborting operation, nothing was modified");

        return;
    }

    i_prDrawListResize(drawList, drawList->meshCount + meshCount);

    for(size_t i = 0; i < meshCount; i++) {
        drawList->meshes[drawList->meshCount] = meshes[i];
        drawList->meshCount++;
    }
}

void prDrawListPop(prDrawList* drawList) {
    if(drawList->meshCount == 0) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "No meshes to pop from the draw list. Aborting operation, nothing was modified");

        return;
    }

    if(drawList->meshCount > 0) {
        i_prDrawListResize(drawList, drawList->meshCount - 1);
    }
    drawList->meshCount--;
}

void prDrawListPopMultiple(prDrawList* drawList, size_t meshCount) {
    if(drawList->meshCount == 0 || meshCount == 0 || meshCount > drawList->meshCount) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "No meshes to pop from the draw list or trying to pop too few or many items. Aborting operation, nothing was modified");

        return;
    }

    if(drawList->meshCount > 0) {
        i_prDrawListResize(drawList, drawList->meshCount - meshCount);
    }
    drawList->meshCount -= meshCount;
}

void prDrawListPopAt(prDrawList* drawList, size_t index) {
    if(drawList->meshCount == 0 || index >= drawList->meshCount) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "No meshes to pop from the draw list or trying to pop item out of bounds. Aborting operation, nothing was modified");

        return;
    }

    memmove(drawList->meshes + index * sizeof(prMeshData*),
            drawList->meshes + (index + 1) * sizeof(prMeshData*),
            (drawList->meshCount - index - 1) * sizeof(prMeshData*));

    if(drawList->meshCount > 0) {
        i_prDrawListResize(drawList, drawList->meshCount - 1);
    }
    drawList->meshCount--;
}

void prDrawListPopAll(prDrawList* drawList) {
    if(drawList->meshCount == 0) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "No meshes to pop from the draw list. Aborting operation, nothing was modified");

        return;
    }

    if(drawList->meshCount > 0) {
        prFree(drawList->meshes);
    }
    drawList->meshCount = 0;
}

void prDrawListOrder(prDrawList* drawList) {
    if(drawList->isOrdered) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "Too few meshes to sort or draw list is already ordered. Aborting operation, nothing was modified");

        return;
    }

    // Sort.

    drawList->isOrdered = true;
}
