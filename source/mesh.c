#include <PR/mesh.h>

#include <PR/meshInternal.h>

#include <PR/memory.h>
#include <PR/logger.h>

prMeshData* prMeshCreate() {
    prMeshData* mesh = prCalloc(1, sizeof(prMeshData));

    return mesh;
}

void prMeshDestroy(prMeshData* mesh) {
    if(mesh->VAO) {
        i_prMeshDestroyOnGPU(mesh);
    }

    if(mesh->GPUReadyBuffer) {
        prFree(mesh->GPUReadyBuffer);
        prFree(mesh->indices);
    }

    prFree(mesh);
}

void prMeshLinkContext(prMeshData* mesh, GladGLContext* context) {
    if(mesh->context && mesh->GPUReadyBuffer) {
        i_prMeshDestroyOnGPU(mesh);
    }
    mesh->context = context;
    if(mesh->context && mesh->GPUReadyBuffer) {
        i_prMeshCreateOnGPU(mesh);
    }
}

void prMeshUpdate(prMeshData* mesh, void* GPUReadyBuffer, GLsizeiptr GPUReadyBufferSize, void* indices, GLsizeiptr indicesSize) {
    if(!GPUReadyBufferSize) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Mesh data size cannot be zero. Aborting operation, nothing was modified");
        return;
    }
    if(!GPUReadyBuffer) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Mesh data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(!indicesSize) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Indices data size cannot be zero. Aborting operation, nothing was modified");
        return;
    }
    if(!indices) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Indices data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(mesh->GPUReadyBuffer) {
        prFree(mesh->GPUReadyBuffer);
        prFree(mesh->indices);
        mesh->GPUReadyBuffer = NULL;
        mesh->indices = NULL;
    }

    mesh->GPUReadyBuffer = prMalloc(GPUReadyBufferSize);
    prMemcpy(mesh->GPUReadyBuffer, GPUReadyBuffer, GPUReadyBufferSize);
    mesh->GPUReadyBufferSize = GPUReadyBufferSize;

    mesh->indices = prMalloc(indicesSize);
    prMemcpy(mesh->indices, indices, indicesSize);
    mesh->indicesSize = indicesSize;

    if(mesh->context && !mesh->VAO) {
        i_prMeshCreateOnGPU(mesh);
    } else if(mesh->context) {
        i_prMeshUpdateOnGPU(mesh);
    }
}

void prMeshDraw(prMeshData* mesh) {
    if(!mesh->context) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "prMeshDraw: Cannot draw mesh without a valid OpenGL context. Aborting operation, nothing was modified");
        return;
    }
    if(!mesh->VAO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "prMeshDraw: Cannot draw mesh without a valid OpenGL VAO. Aborting operation, nothing was modified");
        return;
    }

    i_prMeshDrawOnGPU(mesh);
}
