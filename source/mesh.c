#include <PR/mesh.h>

#include <PR/meshInternal.h>

#include <PR/memory.h>
#include <PR/material.h>
#include <PR/texture.h>
#include <PR/camera.h>
#include <PR/shader.h>
#include <PR/logger.h>

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
    GLuint indices[], size_t indicesCount, 
    GLfloat textureCoordinates[], size_t textureCoordinatesCount) {
    if(!verticesCount) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Vertices data count cannot be zero. Aborting operation, nothing was modified");
        return;
    } else if(verticesCount % 3 != 0) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Vertices data count must be a multiple of 3 (was %zu). Aborting operation, nothing was modified", verticesCount);
        return;
    } else if(!vertices) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Vertices data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(!indicesCount) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Indices data count cannot be zero. Aborting operation, nothing was modified");
        return;
    } else if(indicesCount % 3 != 0) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Indices data count must be a multiple of 3 (was %zu). Aborting operation, nothing was modified", indicesCount);
        return;
    }
    if(!indices) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Indices data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(textureCoordinatesCount / 2 != verticesCount / 3) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Texture coordinates data not enough for every vertex (was %zu). Aborting operation, nothing was modified", textureCoordinatesCount);
        return;
    }
    if(!textureCoordinates) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Texture coordinates data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

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
        i_prMeshCreateOnGPU(mesh);
    } else if(mesh->context) {
        i_prMeshUpdateOnGPU(mesh);
    }
}

void prMeshDraw(prMeshData* mesh, mat4 translation, prCamera* camera, prShaderProgramData* shaderProgram) {
    GladGLContext* context = mesh->context;

    static prMaterialData defaultMaterial = {.shininess = -255.0f};
    prMaterialData* material = mesh->material;

    if(!mesh->material) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "Material not set, using default material");
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
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Material specular map context does not match mesh context. Aborting operation, nothing was modified");
            return;
        }
    }

    context->UseProgram(shaderProgram->shaderProgramObject);

    context->BindVertexArray(mesh->VAO);

    if(mesh->material->ambientMap) {
        if(mesh->material->ambientMap->TBO) {
            context->ActiveTexture(GL_TEXTURE0);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->ambientMap->TBO);
        } else {
            context->ActiveTexture(GL_TEXTURE0);
            context->BindTexture(GL_TEXTURE_2D, 0);
        }
    }
    if(mesh->material->diffuseMap) {
        if(mesh->material->diffuseMap->TBO) {
            context->ActiveTexture(GL_TEXTURE1);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->diffuseMap->TBO);
        } else {
            context->ActiveTexture(GL_TEXTURE1);
            context->BindTexture(GL_TEXTURE_2D, 0);
        }
    }
    if(mesh->material->specularMap) {
        if(mesh->material->specularMap->TBO) {
            context->ActiveTexture(GL_TEXTURE2);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->specularMap->TBO);
        } else {
            context->ActiveTexture(GL_TEXTURE2);
            context->BindTexture(GL_TEXTURE_2D, 0);
        }
    }
    if(mesh->material->normalMap) {
        if(mesh->material->normalMap->TBO) {
            context->ActiveTexture(GL_TEXTURE3);
            context->BindTexture(GL_TEXTURE_2D, mesh->material->normalMap->TBO);
        } else {
            context->ActiveTexture(GL_TEXTURE3);
            context->BindTexture(GL_TEXTURE_2D, 0);
        }
    }

    prShaderProgramUniform3f(shaderProgram, "cameraPosition", camera->position[0], camera->position[1], camera->position[2]);

    prShaderProgramUniformMatrix4fv(shaderProgram, "view", camera->view[0]);

    prShaderProgramUniformMatrix4fv(shaderProgram, "projection", camera->projection[0]);

    prShaderProgramUniformMatrix4fv(shaderProgram, "translation", translation[0]);

    prShaderProgramUniform1i(shaderProgram, "material.ambient", 0);
    prShaderProgramUniform1i(shaderProgram, "material.diffuse", 1);
    prShaderProgramUniform1i(shaderProgram, "material.specular", 2);
    prShaderProgramUniform1i(shaderProgram, "material.normal", 3);
    prShaderProgramUniform1f(shaderProgram, "material.shininess", material->shininess);

    context->DrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);

    context->BindVertexArray(0);
}
