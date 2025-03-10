#include <PR/mesh.h>

#include <PR/meshInternal.h>

#include <PR/memory.h>
#include <PR/texture.h>
#include <PR/error.h>

prMeshData* prMeshCreate() {
    prMeshData* mesh = prMalloc(sizeof(prMeshData));

    mesh->context = NULL;
    mesh->vertices = NULL;
    mesh->textureCoordinates = NULL;
    mesh->vertexColor = NULL;
    mesh->indices = NULL;
    mesh->verticesCount = 0;
    mesh->textureCoordinatesCount = 0;
    mesh->vertexColorCount = 0;
    mesh->indicesCount = 0;
    mesh->VBO = 0;
    mesh->VAO = 0;
    mesh->EBO = 0;
    mesh->GPUReadyBuffer = NULL;
    mesh->GPUReadyBufferCount = 0;
    mesh->mixRatio = 0.0f;

    return mesh;
}

void prMeshDestroy(prMeshData* mesh) {
    i_prMeshDestroyOnGPUSide(mesh);
    prFree(mesh);
}

void prMeshLinkWindow(prMeshData* mesh, GladGLContext* context) {
    if(mesh->context && mesh->GPUReadyBuffer) {
        i_prMeshDestroyOnGPUSide(mesh);
    }
    mesh->context = context;
    if(mesh->context && mesh->GPUReadyBuffer) {
        i_prMeshCreateOnGPUSide(mesh);
    }
}

void prMeshLinkTexture(prMeshData* mesh, prTextureData* texture) {
    if(texture->context != mesh->context) {
        prError(PR_GL_ERROR, "Texture context does not match mesh context. Aborting operation, nothing was modified");
        return;
    }

    mesh->texture = texture;
}

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, GLuint indices[], size_t indicesCount, GLfloat textureCoordinates[], size_t textureCoordinatesCount, GLfloat vertexColor[], size_t vertexColorCount) {
    if(mesh->vertices) {
        prFree(mesh->vertices);
        prFree(mesh->indices);
        mesh->vertices = NULL;
        mesh->indices = NULL;
    }
    if(mesh->textureCoordinates) {
        prFree(mesh->textureCoordinates);
        mesh->textureCoordinates = NULL;
    }

    if(!verticesCount) {
        prError(PR_INVALID_DATA_ERROR, "Vertices data count cannot be zero. Aborting operation, nothing was modified");
        return;
    } else if(verticesCount % 3 != 0) {
        prError(PR_INVALID_DATA_ERROR, "Vertices data count must be a multiple of 3. Aborting operation, nothing was modified");
        return;
    } else if(!vertices) {
        prError(PR_INVALID_DATA_ERROR, "Vertices data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(!indicesCount) {
        prError(PR_INVALID_DATA_ERROR, "Indices data count cannot be zero. Aborting operation, nothing was modified");
        return;
    } else if(indicesCount % 2 != 0) {
        prError(PR_INVALID_DATA_ERROR, "Indices data count must be a multiple of 2. Aborting operation, nothing was modified");
        return;
    } else if(!indices) {
        prError(PR_INVALID_DATA_ERROR, "Indices data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(textureCoordinatesCount % 2 != 0) {
        prError(PR_INVALID_DATA_ERROR, "Texture coordinates data count must be a multiple of 2. Aborting operation, nothing was modified");
        return;
    }
    if(!textureCoordinates && textureCoordinatesCount) {
        prError(PR_INVALID_DATA_ERROR, "Texture coordinates data cannot be NULL while texture coordinates data count is not 0. Aborting operation, nothing was modified");
        return;
    } else if(textureCoordinatesCount / 2 != verticesCount / 3 && textureCoordinatesCount != 0) {
        prError(PR_INVALID_DATA_ERROR, "Texture coordinates data not enough for every vertex. Aborting operation, nothing was modified");
        return;
    }

    if(vertexColorCount % 4 != 0) {
        prError(PR_INVALID_DATA_ERROR, "Vertex color data count must be a multiple of 4. Aborting operation, nothing was modified");
        return;
    } else if(vertexColorCount / 4 != verticesCount / 3 && vertexColorCount != 0) {
        prError(PR_INVALID_DATA_ERROR, "Vertex color data not enough for every vertex. Aborting operation, nothing was modified");
        return;
    }
    if(!vertexColor && vertexColorCount) {
        prError(PR_INVALID_DATA_ERROR, "Vertex color data cannot be NULL while texture coordinates data count is not 0. Aborting operation, nothing was modified");
        return;
    }

    mesh->vertices = prMalloc(sizeof(GLfloat) * verticesCount);
    prMemcpy(mesh->vertices, vertices, sizeof(GLfloat) * verticesCount);
    mesh->verticesCount = verticesCount;

    mesh->indices = prMalloc(sizeof(GLuint) * indicesCount);
    prMemcpy(mesh->indices, indices, sizeof(GLuint) * indicesCount);
    mesh->indicesCount = indicesCount;

    if(textureCoordinatesCount && textureCoordinates) {
        mesh->textureCoordinates = prMalloc(sizeof(GLfloat) * textureCoordinatesCount);
        prMemcpy(mesh->textureCoordinates, textureCoordinates, sizeof(GLfloat) * textureCoordinatesCount);
    }
    mesh->textureCoordinatesCount = textureCoordinatesCount;

    if(vertexColorCount && vertexColor) {
        mesh->vertexColor = prMalloc(sizeof(GLfloat) * vertexColorCount);
        prMemcpy(mesh->vertexColor, vertexColor, sizeof(GLfloat) * vertexColorCount);
    }
    mesh->vertexColorCount = vertexColorCount;

    if(mesh->context && !mesh->VAO) {
        i_prMeshCreateOnGPUSide(mesh);
    } else if(mesh->context) {
        i_prMeshUpdateOnGPUSide(mesh);
    }
}

void prMeshTextureToColorRatio(prMeshData* mesh, float mixRatio) {
    mesh->mixRatio = mixRatio;
}

void prMeshDraw(prMeshData* mesh, unsigned int shaderProgram) {
    GladGLContext* context = mesh->context;
    prTextureData* texture = mesh->texture;

    context->UseProgram(shaderProgram);

    context->BindVertexArray(mesh->VAO);

    if(mesh->textureCoordinatesCount) {
        context->ActiveTexture(GL_TEXTURE0);
        context->BindTexture(GL_TEXTURE_2D, texture->TBO);
    }
    int mixRatioLocation = context->GetUniformLocation(shaderProgram, "mixRatio");
    context->Uniform1f(mixRatioLocation, mesh->mixRatio);

    context->DrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);

    context->BindVertexArray(0);
}
