#include <PR/meshInternal.h>

#include <glad/gl.h>
#include <PR/memory.h>
#include <PR/logger.h>
#include <PR/mesh.h>
#include <PR/shader.h>
#include <PR/texture.h>
#include <PR/camera.h>

void i_prMeshCreateOnGPU(prMeshData* mesh) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prMeshCreateOnGPU: Creating mesh on GPU. Vertices data size: %i indices data size: %i", mesh->GPUReadyBufferSize, mesh->indicesSize);

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

    GLint stride = 8;

    mesh->context->BindVertexArray(mesh->VAO);

    mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferSize * sizeof(GLfloat), mesh->GPUReadyBuffer, GL_STATIC_DRAW);

    mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesSize * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);

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
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prMeshUpdateOnGPU: Updating mesh on GPU. Vertices data size: %i indices data size: %i", mesh->GPUReadyBufferSize, mesh->indicesSize);

    GLint stride = 8;

	mesh->context->BindVertexArray(mesh->VAO);

	mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferSize, mesh->GPUReadyBuffer, GL_STATIC_DRAW);

	mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesSize, mesh->indices, GL_STATIC_DRAW);

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

void i_prMeshDrawOnGPU(prMeshData* mesh) {
    mesh->context->BindVertexArray(mesh->VAO);

    mesh->context->DrawElements(GL_TRIANGLES, mesh->indicesSize, GL_UNSIGNED_INT, 0);

    mesh->context->BindVertexArray(0);
}
