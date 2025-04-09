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

    if(mesh->vertices) {
        prFree(mesh->vertices);
    }
    if(mesh->textureCoordinates) {
        prFree(mesh->textureCoordinates);
    }
    if(mesh->indices) {
        prFree(mesh->indices);
    }
    if(mesh->GPUReadyBuffer) {
        prFree(mesh->GPUReadyBuffer);
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

void prMeshLinkMaterial(prMeshData* mesh, prMaterialData* material) {
    mesh->material = material;
}

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount,
    GLfloat normals[], size_t normalsCount,
    GLfloat textureCoordinates[], size_t textureCoordinatesCount,
    GLuint indices[], size_t indicesCount) {
    if(!verticesCount) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Vertices data count cannot be zero. Aborting operation, nothing was modified");
        return;
    } else if(verticesCount % 3 != 0) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Vertices data count must be a multiple of 3 (was %zu). Aborting operation, nothing was modified", verticesCount);
        return;
    } else if(!vertices) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Vertices data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(normalsCount < verticesCount) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Normals data not enough for every vertex (was %zu). Aborting operation, nothing was modified", normalsCount);
        return;
    }
    if(!normals) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Normals data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(textureCoordinatesCount / 2 != verticesCount / 3 && textureCoordinatesCount < verticesCount) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Texture coordinates data not enough for every vertex (was %zu). Aborting operation, nothing was modified", textureCoordinatesCount);
        return;
    }
    if(!textureCoordinates) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Texture coordinates data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(!indicesCount) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Indices data count cannot be zero. Aborting operation, nothing was modified");
        return;
    } else if(indicesCount % 3 != 0) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Indices data count must be a multiple of 3 (was %zu). Aborting operation, nothing was modified", indicesCount);
        return;
    }
    if(!indices) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prMeshUpdate: Indices data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(mesh->vertices) {
        prFree(mesh->vertices);
        prFree(mesh->indices);
        mesh->vertices = NULL;
        mesh->indices = NULL;
    }
    if(mesh->normals) {
        prFree(mesh->normals);
        mesh->normals = NULL;
    }
    if(mesh->textureCoordinates) {
        prFree(mesh->textureCoordinates);
        mesh->textureCoordinates = NULL;
    }

    mesh->vertices = prMalloc(sizeof(GLfloat) * verticesCount);
    prMemcpy(mesh->vertices, vertices, sizeof(GLfloat) * verticesCount);
    mesh->verticesCount = verticesCount;

    mesh->normals = prMalloc(sizeof(GLfloat) * normalsCount);
    prMemcpy(mesh->normals, normals, sizeof(GLfloat) * normalsCount);
    mesh->normalsCount = normalsCount;

    mesh->textureCoordinates = prMalloc(sizeof(GLfloat) * textureCoordinatesCount);
    prMemcpy(mesh->textureCoordinates, textureCoordinates, sizeof(GLfloat) * textureCoordinatesCount);
    mesh->textureCoordinatesCount = textureCoordinatesCount;

    mesh->indices = prMalloc(sizeof(GLuint) * indicesCount);
    prMemcpy(mesh->indices, indices, sizeof(GLuint) * indicesCount);
    mesh->indicesCount = indicesCount;

    if(mesh->context && !mesh->VAO) {
        i_prMeshCreateOnGPU(mesh);
    } else if(mesh->context) {
        i_prMeshUpdateOnGPU(mesh);
    }
}

void prMeshDraw(prMeshData* mesh, mat4 translation, prCamera* camera, prShaderData* shaderProgram) {
    if(!mesh->context) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "prMeshDraw: Cannot draw mesh without a valid OpenGL context. Aborting operation, nothing was modified");
        return;
    }
    if(!mesh->VAO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "prMeshDraw: Cannot draw mesh without a valid OpenGL VAO. Aborting operation, nothing was modified");
        return;
    }

    i_prMeshDrawOnGPU(mesh, translation, camera, shaderProgram);
}
