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
    mesh->vertexColor = NULL;
    mesh->vertexNormals = NULL;
    mesh->indices = NULL;
    mesh->verticesCount = 0;
    mesh->textureCoordinatesCount = 0;
    mesh->vertexColorCount = 0;
    mesh->vertexNormalsCount = 0;
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

    if(mesh->vertices) {
        prFree(mesh->vertices);
    }
    if(mesh->textureCoordinates) {
        prFree(mesh->textureCoordinates);
    }
    if(mesh->vertexColor) {
        prFree(mesh->vertexColor);
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
    GLfloat textureCoordinates[], size_t textureCoordinatesCount, 
    GLfloat vertexColor[], size_t vertexColorCount, 
    GLfloat vertexNormals[], size_t vertexNormalsCount) {
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
        prError(PR_INVALID_DATA_ERROR, "Vertex color data cannot be NULL while vertex color data count is not 0. Aborting operation, nothing was modified");
        return;
    }

    if(vertexNormalsCount % 3 != 0) {
        prError(PR_INVALID_DATA_ERROR, "Vertex normal data count must be a multiple of 3. Aborting operation, nothing was modified");
        return;
    }
    if(!vertexNormals && vertexNormalsCount) {
        prError(PR_INVALID_DATA_ERROR, "Vertex normal data cannot be NULL while vertex normals data count is not 0. Aborting operation, nothing was modified");
        return;
    } else if(vertexNormalsCount != verticesCount && vertexNormalsCount != 0) {
        prError(PR_INVALID_DATA_ERROR, "Vertex normal data not enough for every vertex. Aborting operation, nothing was modified");
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

    if(vertexNormalsCount && vertexNormals) {
        mesh->vertexNormals = prMalloc(sizeof(GLfloat) * vertexNormalsCount);
        prMemcpy(mesh->vertexNormals, vertexNormals, sizeof(GLfloat) * vertexNormalsCount);
    }
    mesh->vertexNormalsCount = vertexNormalsCount;

    if(mesh->context && !mesh->VAO) {
        i_prMeshCreateOnGPUSide(mesh);
    } else if(mesh->context) {
        i_prMeshUpdateOnGPUSide(mesh);
    }
}

void prMeshTextureToColorRatio(prMeshData* mesh, float mixRatio) {
    mesh->mixRatio = mixRatio;
}

void prMeshDraw(prMeshData* mesh, mat4 translation, prCamera* camera,  unsigned int shaderProgram) {
    GladGLContext* context = mesh->context;

    static prMaterialData defaultMaterial = {.shininess = -255.0f};
    prMaterialData* material = mesh->material;

    if(!mesh->material) {
        prLogWarning("[GL]", "Material not set, using default material");
        if(defaultMaterial.shininess == -255.0f) {
            defaultMaterial.texture = NULL;
            defaultMaterial.specularMap = NULL;
            defaultMaterial.ambientMap = NULL;
            defaultMaterial.diffuseMap = NULL;
            defaultMaterial.specularMap = NULL;
            defaultMaterial.shininess = 32.0f;
            defaultMaterial.ambientStrength = 0.15f;
            defaultMaterial.diffuseStrength = 1.0f;
            defaultMaterial.specularStrength = 0.5f;
        }

        material = &defaultMaterial;
    }

    if(material->texture) {
        if(mesh->material->texture->context != context) {
            prError(PR_GL_ERROR, "Material texture context does not match mesh context. Aborting operation, nothing was modified");
            return;
        }
    }
    if(material->specularMap) {
        if(mesh->material->specularMap->context != context) {
            prError(PR_GL_ERROR, "Material specular map context does not match mesh context. Aborting operation, nothing was modified");
            return;
        }
    }

    context->UseProgram(shaderProgram);

    context->BindVertexArray(mesh->VAO);

    if(mesh->textureCoordinatesCount && mesh->material->texture) {
        if(mesh->material->texture->TBO && mesh->material->texture->context == context) {
            context->ActiveTexture(GL_TEXTURE0);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->texture->TBO);
        }
    }
    if(mesh->textureCoordinatesCount && mesh->material->ambientMap) {
        if(mesh->material->ambientMap->TBO) {
            context->ActiveTexture(GL_TEXTURE1);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->ambientMap->TBO);
        }
    }
    if(mesh->textureCoordinatesCount && mesh->material->diffuseMap) {
        if(mesh->material->diffuseMap->TBO) {
            context->ActiveTexture(GL_TEXTURE2);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->diffuseMap->TBO);
        }
    }
    if(mesh->textureCoordinatesCount && mesh->material->specularMap) {
        if(mesh->material->specularMap->TBO) {
            context->ActiveTexture(GL_TEXTURE3);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->specularMap->TBO);
        }
    }

    int viewUniformLocation = context->GetUniformLocation(shaderProgram, "view");
    context->UniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, camera->view[0]);

    int projectionUniformLocation = context->GetUniformLocation(shaderProgram, "projection");
    context->UniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, camera->projection[0]);

    int translationUniformLocation = context->GetUniformLocation(shaderProgram, "translation");
    context->UniformMatrix4fv(translationUniformLocation, 1, GL_FALSE, translation[0]);

    int mixRatioUniformLocation = context->GetUniformLocation(shaderProgram, "mixRatio");
    context->Uniform1f(mixRatioUniformLocation, mesh->mixRatio);

    int blendAlphaUniformLocation = context->GetUniformLocation(shaderProgram, "blendAlpha");
    context->Uniform1i(blendAlphaUniformLocation, 0);

    int cameraPositionUniformLocation = context->GetUniformLocation(shaderProgram, "cameraPosition");
    context->Uniform3f(cameraPositionUniformLocation, camera->position[0], camera->position[1], camera->position[2]);

    int ambientUniformLocation = context->GetUniformLocation(shaderProgram, "material.ambient");
    context->Uniform1i(ambientUniformLocation, 1);
    int diffuseUniformLocation = context->GetUniformLocation(shaderProgram, "material.diffuse");
    context->Uniform1i(diffuseUniformLocation, 2);
    int specularUniformLocation = context->GetUniformLocation(shaderProgram, "material.specular");
    context->Uniform1i(specularUniformLocation, 3);
    int shininessUniformLocation = context->GetUniformLocation(shaderProgram, "material.shininess");
    context->Uniform1f(shininessUniformLocation, material->shininess);

    int ambientColorStrengthUniformLocation = context->GetUniformLocation(shaderProgram, "material.ambientStrength");
    context->Uniform1f(ambientColorStrengthUniformLocation, material->ambientStrength);
    int diffuseStrengthUniformLocation = context->GetUniformLocation(shaderProgram, "material.diffuseStrength");
    context->Uniform1f(diffuseStrengthUniformLocation, material->diffuseStrength);
    int specularStrengthUniformLocation = context->GetUniformLocation(shaderProgram, "material.specularStrength");
    context->Uniform1f(specularStrengthUniformLocation, material->specularStrength);

    context->DrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);

    context->BindVertexArray(0);
}
