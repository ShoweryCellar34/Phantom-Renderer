#include <PR/meshInternal.h>

#include <glad/gl.h>
#include <PR/memory.h>
#include <PR/logger.h>
#include <PR/mesh.h>
#include <PR/material.h>
#include <PR/shader.h>
#include <PR/texture.h>
#include <PR/camera.h>

void i_prMeshComputeGPUReadyBuffer(prMeshData* mesh) {
    if (mesh->GPUReadyBuffer) {
        prFree(mesh->GPUReadyBuffer);
        mesh->GPUReadyBuffer = NULL;
    }

    mesh->GPUReadyBufferCount = mesh->verticesCount + mesh->normalsCount + mesh->textureCoordinatesCount;
    mesh->GPUReadyBuffer = prMalloc(mesh->GPUReadyBufferCount * sizeof(GLfloat));

    size_t index = 0;
    for(size_t i = 0; i < mesh->verticesCount / 3; i++) {
        mesh->GPUReadyBuffer[index++] = mesh->vertices[i * 3];
        mesh->GPUReadyBuffer[index++] = mesh->vertices[i * 3 + 1];
        mesh->GPUReadyBuffer[index++] = mesh->vertices[i * 3 + 2];
        // printf("Vertex %i:\n    XYZ:  %.3f, %.3f, %.3f\n", i, mesh->GPUReadyBuffer[index - 3], mesh->GPUReadyBuffer[index - 2], mesh->GPUReadyBuffer[index - 1]);

        mesh->GPUReadyBuffer[index++] = mesh->normals[i * 3];
        mesh->GPUReadyBuffer[index++] = mesh->normals[i * 3 + 1];
        mesh->GPUReadyBuffer[index++] = mesh->normals[i * 3 + 2];
        // printf("NORMAL:  %.3f, %.3f, %.3f\n", i, mesh->GPUReadyBuffer[index - 3], mesh->GPUReadyBuffer[index - 2], mesh->GPUReadyBuffer[index - 1]);

        mesh->GPUReadyBuffer[index++] = mesh->textureCoordinates[i * 2];
        mesh->GPUReadyBuffer[index++] = mesh->textureCoordinates[i * 2 + 1];
        // printf("    UV:   %.3f, %.3f\n", mesh->GPUReadyBuffer[index - 2], mesh->GPUReadyBuffer[index - 1]);
    }
}

void i_prMeshCreateOnGPU(prMeshData* mesh) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prMeshCreateOnGPU: Creating mesh on GPU. Verticies: %i Normals: %i Texture coordinates: %i Indices: %i", mesh->verticesCount, mesh->normalsCount, mesh->textureCoordinatesCount, mesh->indicesCount);

    mesh->context->GenVertexArrays(1, &mesh->VAO);
    if(!mesh->VAO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prMeshCreateOnGPU: Failed to create vertex array object. Aborting operation, nothing was modified");
        return;
    }

    mesh->context->GenBuffers(1, &mesh->VBO);
    if(!mesh->VBO) {
        mesh->context->DeleteVertexArrays(1, &mesh->VAO);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prMeshCreateOnGPU: Failed to create vertex buffer object. Aborting operation, nothing was modified");
        return;
    }

    mesh->context->GenBuffers(1, &mesh->EBO);
    if(!mesh->EBO) {
        mesh->context->DeleteVertexArrays(1, &mesh->VAO);
        mesh->context->DeleteBuffers(1, &mesh->VBO);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prMeshCreateOnGPU: Failed to create index buffer object. Aborting operation, nothing was modified");
        return;
    }

    i_prMeshComputeGPUReadyBuffer(mesh);

    int stride = 8;

    mesh->context->BindVertexArray(mesh->VAO);

    mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount * sizeof(GLfloat), mesh->GPUReadyBuffer, GL_STATIC_DRAW);

    mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);

    mesh->context->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    mesh->context->EnableVertexAttribArray(0);

    mesh->context->VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    mesh->context->EnableVertexAttribArray(1);

    mesh->context->VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    mesh->context->EnableVertexAttribArray(2);

    mesh->context->BindBuffer(GL_ARRAY_BUFFER, 0);

    mesh->context->BindVertexArray(0);
}

void i_prMeshUpdateOnGPU(prMeshData* mesh) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prMeshUpdateOnGPU: Updating mesh on GPU. Verticies: %i Normals: %i Texture coordinates: %i Indices: %i", mesh->verticesCount, mesh->normalsCount, mesh->textureCoordinatesCount, mesh->indicesCount);

    i_prMeshComputeGPUReadyBuffer(mesh);

    int stride = 8;

	mesh->context->BindVertexArray(mesh->VAO);

	mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount, mesh->GPUReadyBuffer, GL_STATIC_DRAW);

	mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount, mesh->indices, GL_STATIC_DRAW);

    mesh->context->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    mesh->context->EnableVertexAttribArray(0);

    mesh->context->VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    mesh->context->EnableVertexAttribArray(1);

    mesh->context->VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    mesh->context->EnableVertexAttribArray(2);

	mesh->context->BindVertexArray(0);

    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prMeshUpdateOnGPU: Successfully updated vertex array object and set data");
}

void i_prMeshDestroyOnGPU(prMeshData* mesh) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prMeshDestroyOnGPU: Destroying mesh on GPU");

    mesh->context->DeleteVertexArrays(1, &mesh->VAO);
    mesh->context->DeleteBuffers(1, &mesh->VBO);
    mesh->context->DeleteBuffers(1, &mesh->EBO);
}

void i_prMeshDrawOnGPU(prMeshData* mesh, mat4 translation, prCamera* camera, prShaderData* shaderProgram) {
    if(!shaderProgram->context) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prMeshDrawOnGPU: Cannot use shader without a valid OpenGL context. Aborting operation, nothing was modified");
        return;
    }
    if(mesh->context != shaderProgram->context) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prMeshDrawOnGPU: Mesh and shader contexts do not match. Aborting operation, nothing was modified");
        return;
    }
    if(!shaderProgram->shaderProgramObject) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prMeshDrawOnGPU: Invalid shader program. Aborting operation, nothing was modified");
        return;
    }

    prMaterialData defaultMaterial = {NULL, NULL, NULL, NULL, 32.0f};
    prMaterialData* material = mesh->material;

    if(!mesh->material) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "i_prMeshDrawOnGPU: Material not set, using default material");
        material = &defaultMaterial;
    }

    mesh->context->UseProgram(shaderProgram->shaderProgramObject);
    mesh->context->BindVertexArray(mesh->VAO);

    mesh->context->ActiveTexture(GL_TEXTURE0);
    if(mesh->material->ambientMap) {
        if(mesh->material->ambientMap->TBO) {
            if(mesh->material->ambientMap->context != mesh->context) {
                prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prMeshDrawOnGPU: Ambient map context does not match mesh context, binding ID 0");
            } else {
                mesh->context->BindTexture(GL_TEXTURE_2D, mesh->material->ambientMap->TBO);
            }
        } else {
            mesh->context->BindTexture(GL_TEXTURE_2D, 0);
        }
    } else {
        mesh->context->BindTexture(GL_TEXTURE_2D, 0);
    }
    mesh->context->ActiveTexture(GL_TEXTURE1);
    if(mesh->material->diffuseMap) {
        if(mesh->material->diffuseMap->TBO) {
            if(mesh->material->diffuseMap->context != mesh->context) {
                prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prMeshDrawOnGPU: Diffuse map context does not match mesh context, binding ID 0");
            } else {
                mesh->context->BindTexture(GL_TEXTURE_2D, mesh->material->diffuseMap->TBO);
            }
        } else {
            mesh->context->BindTexture(GL_TEXTURE_2D, 0);
        }
    } else {
        mesh->context->BindTexture(GL_TEXTURE_2D, 0);
    }
    mesh->context->ActiveTexture(GL_TEXTURE2);
    if(mesh->material->specularMap) {
        if(mesh->material->specularMap->TBO) {
            if(mesh->material->specularMap->context != mesh->context) {
                prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prMeshDrawOnGPU: Specular map context does not match mesh context, binding ID 0");
            } else {
                mesh->context->BindTexture(GL_TEXTURE_2D, mesh->material->specularMap->TBO);
            }
        } else {
            mesh->context->BindTexture(GL_TEXTURE_2D, 0);
        }
    } else {
        mesh->context->BindTexture(GL_TEXTURE_2D, 0);
    }
    mesh->context->ActiveTexture(GL_TEXTURE3);
    if(mesh->material->normalMap) {
        if(mesh->material->normalMap->TBO) {
            if(mesh->material->normalMap->context != mesh->context) {
                prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prMeshDrawOnGPU: Normal map context does not match mesh context, binding ID 0");
            } else {
                mesh->context->BindTexture(GL_TEXTURE_2D, mesh->material->normalMap->TBO);
            }
        } else {
            mesh->context->BindTexture(GL_TEXTURE_2D, 0);
        }
    } else {
        mesh->context->BindTexture(GL_TEXTURE_2D, 0);
    }

    prShaderUniform3f(shaderProgram, "cameraPosition", camera->position[0], camera->position[1], camera->position[2]);

    prShaderUniformMatrix4fv(shaderProgram, "view", camera->view[0]);

    prShaderUniformMatrix4fv(shaderProgram, "projection", camera->projection[0]);

    prShaderUniformMatrix4fv(shaderProgram, "translation", translation[0]);

    prShaderUniform1i(shaderProgram, "material.ambient", 0);
    prShaderUniform1i(shaderProgram, "material.diffuse", 1);
    prShaderUniform1i(shaderProgram, "material.specular", 2);
    prShaderUniform1i(shaderProgram, "material.normal", 3);
    prShaderUniform1f(shaderProgram, "material.shininess", material->shininess);

    mesh->context->DrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);

    mesh->context->BindVertexArray(0);
}
