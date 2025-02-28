#include <PR/meshInternal.h>

#include <PR/window.h>
#include <glad/gl.h>

void i_prMeshCreateOnGPUSide(prMeshData* mesh) {
        GladGLContext* context = &mesh->window->openglContext;

        context->GenVertexArrays(1, &mesh->VAO);
        context->GenBuffers(1, &mesh->VBO);
        context->GenBuffers(1, &mesh->EBO);
        context->BindVertexArray(mesh->VAO);

        context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount, mesh->GPUReadyBuffer, GL_STATIC_DRAW);

        context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount, mesh->indices, GL_STATIC_DRAW);

        context->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
        context->EnableVertexAttribArray(0);

        context->VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        context->EnableVertexAttribArray(1);

        context->VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
        context->EnableVertexAttribArray(2);

        context->BindVertexArray(0);
}

void i_prMeshUpdateOnGPUSide(prMeshData* mesh) {
        GladGLContext* context = &mesh->window->openglContext;

        context->BindVertexArray(mesh->VAO);

        context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount, mesh->GPUReadyBuffer, GL_STATIC_DRAW);

        context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount, mesh->indices, GL_STATIC_DRAW);

        context->BindVertexArray(0);
}

void i_prMeshDestroyOnGPUSide(prMeshData* mesh) {
        GladGLContext* context = &mesh->window->openglContext;

        context->DeleteVertexArrays(1, &mesh->VAO);
        context->DeleteBuffers(1, &mesh->VBO);
        context->DeleteBuffers(1, &mesh->EBO);
}
