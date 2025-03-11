#include <PR/meshInternal.h>

#include <glad/gl.h>
#include <PR/memory.h>
#include <PR/error.h>

void i_prMeshComputeGPUReadyBuffer(prMeshData* mesh) {
    if(mesh->GPUReadyBuffer) {
        prFree(mesh->GPUReadyBuffer);
        mesh->GPUReadyBuffer = NULL;
    }

    mesh->GPUReadyBufferCount = mesh->verticesCount + mesh->textureCoordinatesCount + mesh->vertexColorCount;
    GLuint GPUReadyBufferAttributeCount = mesh->GPUReadyBufferCount / 4;
    mesh->GPUReadyBuffer = prMalloc(mesh->GPUReadyBufferCount * sizeof(GLfloat));
    for(size_t i = 0; i < mesh->GPUReadyBufferCount; i++) {
        size_t vertexIndex = (i / GPUReadyBufferAttributeCount) * 3;
        mesh->GPUReadyBuffer[i++] = mesh->vertices[vertexIndex];
        mesh->GPUReadyBuffer[i++] = mesh->vertices[vertexIndex + 1];
        mesh->GPUReadyBuffer[i] = mesh->vertices[vertexIndex + 2];

        if(mesh->textureCoordinatesCount) {
            size_t textureCoordinatesIndex = (i++ / GPUReadyBufferAttributeCount) * 2;
            mesh->GPUReadyBuffer[i++] = mesh->textureCoordinates[textureCoordinatesIndex];
            mesh->GPUReadyBuffer[i] = mesh->textureCoordinates[textureCoordinatesIndex + 1];
        }

        if(mesh->vertexColorCount) {
            size_t vertexColorIndex = (i++ / GPUReadyBufferAttributeCount) * 4;
            mesh->GPUReadyBuffer[i++] = mesh->vertexColor[vertexColorIndex];
            mesh->GPUReadyBuffer[i++] = mesh->vertexColor[vertexColorIndex + 1];
            mesh->GPUReadyBuffer[i++] = mesh->vertexColor[vertexColorIndex + 2];
            mesh->GPUReadyBuffer[i] = mesh->vertexColor[vertexColorIndex + 3];
        }
    }
    mesh->GPUReadyBufferCount = mesh->GPUReadyBufferCount;
}

void i_prMeshCreateOnGPUSide(prMeshData* mesh) {
    mesh->context->GenVertexArrays(1, &mesh->VAO);
    if(!mesh->VAO) {
        prError(PR_GL_ERROR, "Failed to create vertex array object. Aborting operation, nothing was modified");
        return;
    }

    mesh->context->GenBuffers(1, &mesh->VBO);
    if(!mesh->VBO) {
        mesh->context->DeleteVertexArrays(1, &mesh->VAO);
        prError(PR_GL_ERROR, "Failed to create vertex buffer object. Aborting operation, nothing was modified");
        return;
    }

    mesh->context->GenBuffers(1, &mesh->EBO);
    if(!mesh->EBO) {
        mesh->context->DeleteVertexArrays(1, &mesh->VAO);
        mesh->context->DeleteBuffers(1, &mesh->VBO);
        prError(PR_GL_ERROR, "Failed to create index buffer object. Aborting operation, nothing was modified");
        return;
    }

    i_prMeshComputeGPUReadyBuffer(mesh);

    mesh->context->BindVertexArray(mesh->VAO);

    mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount * sizeof(GLfloat), mesh->GPUReadyBuffer, GL_STATIC_DRAW);

    mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);

    mesh->context->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->GPUReadyBufferCount / 4 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    mesh->context->EnableVertexAttribArray(0);

    if(mesh->textureCoordinatesCount && mesh->textureCoordinates) {
        mesh->context->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, mesh->GPUReadyBufferCount / 4 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        mesh->context->EnableVertexAttribArray(1);
    }

    if(mesh->vertexColorCount && mesh->vertexColor) {
        mesh->context->VertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mesh->GPUReadyBufferCount / 4 * sizeof(GLfloat), (void*)((mesh->textureCoordinatesCount ? 5 : 3) * sizeof(GLfloat)));
        mesh->context->EnableVertexAttribArray(2);
    }

    mesh->context->BindBuffer(GL_ARRAY_BUFFER, 0);

    mesh->context->BindVertexArray(0);
}

void i_prMeshUpdateOnGPUSide(prMeshData* mesh) {
    i_prMeshComputeGPUReadyBuffer(mesh);

	mesh->context->BindVertexArray(mesh->VAO);

	mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount, mesh->GPUReadyBuffer, GL_STATIC_DRAW);

	mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount, mesh->indices, GL_STATIC_DRAW);

    unsigned int i = 0;

    if(mesh->textureCoordinatesCount && mesh->textureCoordinates) {
        mesh->context->VertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, mesh->GPUReadyBufferCount / 4 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        mesh->context->EnableVertexAttribArray(i++);
    }

    if(mesh->vertexColorCount && mesh->vertexColor) {
        mesh->context->VertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, mesh->GPUReadyBufferCount / 4 * sizeof(GLfloat), (void*)(mesh->textureCoordinatesCount ? 5 : 3 * sizeof(GLfloat)));
        mesh->context->EnableVertexAttribArray(i++);
    }

	mesh->context->BindVertexArray(0);

    prLogInfo("[GL]", "Successfully created vertex array object and set data");
}

void i_prMeshDestroyOnGPUSide(prMeshData* mesh) {
    mesh->context->DeleteVertexArrays(1, &mesh->VAO);
    mesh->context->DeleteBuffers(1, &mesh->VBO);
    mesh->context->DeleteBuffers(1, &mesh->EBO);
}
