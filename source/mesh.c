#include <PR/mesh.h>

#include <PR/meshInternal.h>

#include <PR/memory.h>
#include <PR/texture.h>
#include <PR/camera.h>
#include <PR/error.h>

prMeshData* prMeshCreate() {
    prMeshData* mesh = prMalloc(sizeof(prMeshData));

    mesh->context = NULL;
    mesh->material = NULL;
    mesh->vertices = NULL;
    mesh->textureCoordinates = NULL;
    mesh->indices = NULL;
    mesh->verticesCount = 0;
    mesh->textureCoordinatesCount = 0;
    mesh->indicesCount = 0;
    mesh->VBO = 0;
    mesh->VAO = 0;
    mesh->EBO = 0;
    mesh->GPUReadyBuffer = NULL;
    mesh->GPUReadyBufferCount = 0;

    return mesh;
}

void prMeshDestroy(prMeshData* mesh) {
    i_prMeshDestroyOnGPUSide(mesh);

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
        i_prMeshDestroyOnGPUSide(mesh);
    }
    mesh->context = context;
    if(mesh->context && mesh->GPUReadyBuffer) {
        i_prMeshCreateOnGPUSide(mesh);
    }
}

void prMeshLinkMaterial(prMeshData* mesh, prMaterialData* material) {
    mesh->material = material;
}

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, 
    GLuint indices[], size_t indicesCount, 
    GLfloat textureCoordinates[], size_t textureCoordinatesCount) {
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
    } else if(indicesCount % 3 != 0) {
        prError(PR_INVALID_DATA_ERROR, "Indices data count must be a multiple of 3. Aborting operation, nothing was modified");
        return;
    }
    if(!indices) {
        prError(PR_INVALID_DATA_ERROR, "Indices data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(textureCoordinatesCount / 2 != verticesCount / 3) {
        prError(PR_INVALID_DATA_ERROR, "Texture coordinates data not enough for every vertex. Aborting operation, nothing was modified");
        return;
    }
    if(!textureCoordinates) {
        prError(PR_INVALID_DATA_ERROR, "Texture coordinates data cannot be NULL. Aborting operation, nothing was modified");
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

    if(mesh->context && !mesh->VAO) {
        i_prMeshCreateOnGPUSide(mesh);
    } else if(mesh->context) {
        i_prMeshUpdateOnGPUSide(mesh);
    }
}

void prMeshDraw(prMeshData* mesh, mat4 translation, prCamera* camera,  unsigned int shaderProgram) {
    GladGLContext* context = mesh->context;

    static prMaterialData defaultMaterial = {.shininess = -255.0f};
    prMaterialData* material = mesh->material;

    if(!mesh->material) {
        prLogWarning("[GL]", "Material not set, using default material");
        if(defaultMaterial.shininess == -255.0f) {
            defaultMaterial.specularMap = NULL;
            defaultMaterial.ambientMap = NULL;
            defaultMaterial.diffuseMap = NULL;
            defaultMaterial.specularMap = NULL;
            defaultMaterial.shininess = 32.0f;
        }

        material = &defaultMaterial;
    }

    if(material->specularMap) {
        if(mesh->material->specularMap->context != context) {
            prError(PR_GL_ERROR, "Material specular map context does not match mesh context. Aborting operation, nothing was modified");
            return;
        }
    }

    context->UseProgram(shaderProgram);

    context->BindVertexArray(mesh->VAO);

    if(mesh->material->ambientMap) {
        if(mesh->material->ambientMap->TBO) {
            context->ActiveTexture(GL_TEXTURE0);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->ambientMap->TBO);
        }
    }
    if(mesh->material->diffuseMap) {
        if(mesh->material->diffuseMap->TBO) {
            context->ActiveTexture(GL_TEXTURE1);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->diffuseMap->TBO);
        }
    }
    if(mesh->material->specularMap) {
        if(mesh->material->specularMap->TBO) {
            context->ActiveTexture(GL_TEXTURE2);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->specularMap->TBO);
        }
    }
    if(mesh->material->normalMap) {
        if(mesh->material->normalMap->TBO) {
            context->ActiveTexture(GL_TEXTURE3);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->normalMap->TBO);
        }
    }

    int viewUniformLocation = context->GetUniformLocation(shaderProgram, "view");
    context->UniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, camera->view[0]);

    int projectionUniformLocation = context->GetUniformLocation(shaderProgram, "projection");
    context->UniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, camera->projection[0]);

    int translationUniformLocation = context->GetUniformLocation(shaderProgram, "translation");
    context->UniformMatrix4fv(translationUniformLocation, 1, GL_FALSE, translation[0]);

    int cameraPositionUniformLocation = context->GetUniformLocation(shaderProgram, "cameraPosition");
    context->Uniform3f(cameraPositionUniformLocation, camera->position[0], camera->position[1], camera->position[2]);

    int ambientUniformLocation = context->GetUniformLocation(shaderProgram, "material.ambient");
    context->Uniform1i(ambientUniformLocation, 0);
    int diffuseUniformLocation = context->GetUniformLocation(shaderProgram, "material.diffuse");
    context->Uniform1i(diffuseUniformLocation, 1);
    int specularUniformLocation = context->GetUniformLocation(shaderProgram, "material.specular");
    context->Uniform1i(specularUniformLocation, 2);
    int normalUniformLocation = context->GetUniformLocation(shaderProgram, "material.normal");
    context->Uniform1i(normalUniformLocation, 3);
    int shininessUniformLocation = context->GetUniformLocation(shaderProgram, "material.shininess");
    context->Uniform1f(shininessUniformLocation, material->shininess);

    context->DrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);

    context->BindVertexArray(0);
}
