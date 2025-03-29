#include <PR/meshInternal.h>

#include <glad/gl.h>
#include <PR/memory.h>
#include <PR/logger.h>
#include <PR/mesh.h>

void i_prMeshComputeGPUReadyBuffer(prMeshData* mesh) {
    if (mesh->GPUReadyBuffer) {
        prFree(mesh->GPUReadyBuffer);
        mesh->GPUReadyBuffer = NULL;
    }

    mesh->GPUReadyBufferCount = mesh->verticesCount + mesh->textureCoordinatesCount;
    mesh->GPUReadyBuffer = prMalloc(mesh->GPUReadyBufferCount * sizeof(GLfloat));

    size_t index = 0;
    for(size_t i = 0; i < mesh->verticesCount / 3; i++) {
        mesh->GPUReadyBuffer[index++] = mesh->vertices[i * 3];
        mesh->GPUReadyBuffer[index++] = mesh->vertices[i * 3 + 1];
        mesh->GPUReadyBuffer[index++] = mesh->vertices[i * 3 + 2];
        // printf("Vertex %i:\n  XYZ:  %.3f, %.3f, %.3f\n", i, mesh->GPUReadyBuffer[index - 3], mesh->GPUReadyBuffer[index - 2], mesh->GPUReadyBuffer[index - 1]);

        if(mesh->textureCoordinatesCount) {
            mesh->GPUReadyBuffer[index++] = mesh->textureCoordinates[i * 2];
            mesh->GPUReadyBuffer[index++] = mesh->textureCoordinates[i * 2 + 1];
            // printf("  UV:   %.3f, %.3f\n", mesh->GPUReadyBuffer[index - 2], mesh->GPUReadyBuffer[index - 1]);
        }
    }
}

void i_prMeshCreateOnGPU(prMeshData* mesh) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "Creating mesh on GPU. Verticies: %i Indices: %i, Texture coordinates: %i", mesh->verticesCount, mesh->indicesCount, mesh->textureCoordinatesCount);

    mesh->context->GenVertexArrays(1, &mesh->VAO);
    if(!mesh->VAO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "Failed to create vertex array object. Aborting operation, nothing was modified");
        return;
    }

    mesh->context->GenBuffers(1, &mesh->VBO);
    if(!mesh->VBO) {
        mesh->context->DeleteVertexArrays(1, &mesh->VAO);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "Failed to create vertex buffer object. Aborting operation, nothing was modified");
        return;
    }

    mesh->context->GenBuffers(1, &mesh->EBO);
    if(!mesh->EBO) {
        mesh->context->DeleteVertexArrays(1, &mesh->VAO);
        mesh->context->DeleteBuffers(1, &mesh->VBO);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "Failed to create index buffer object. Aborting operation, nothing was modified");
        return;
    }

    i_prMeshComputeGPUReadyBuffer(mesh);

    int stride = 3;
    stride += mesh->textureCoordinatesCount ? 2 : 0;

    mesh->context->BindVertexArray(mesh->VAO);

    mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount * sizeof(GLfloat), mesh->GPUReadyBuffer, GL_STATIC_DRAW);

    mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);

    mesh->context->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    mesh->context->EnableVertexAttribArray(0);

    if(mesh->textureCoordinatesCount && mesh->textureCoordinates) {
        mesh->context->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        mesh->context->EnableVertexAttribArray(1);
    }

    mesh->context->BindBuffer(GL_ARRAY_BUFFER, 0);

    mesh->context->BindVertexArray(0);
}

void i_prMeshUpdateOnGPU(prMeshData* mesh) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "Updating mesh on GPU. Verticies: %i Indices: %i, Texture coordinates: %i", mesh->verticesCount, mesh->indicesCount, mesh->textureCoordinatesCount);

    i_prMeshComputeGPUReadyBuffer(mesh);

    int stride = 3;
    stride += mesh->textureCoordinatesCount ? 2 : 0;

	mesh->context->BindVertexArray(mesh->VAO);

	mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount, mesh->GPUReadyBuffer, GL_STATIC_DRAW);

	mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount, mesh->indices, GL_STATIC_DRAW);

    unsigned int i = 0;

    if(mesh->textureCoordinatesCount && mesh->textureCoordinates) {
        mesh->context->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        mesh->context->EnableVertexAttribArray(1);
    }

	mesh->context->BindVertexArray(0);

    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "Successfully updated vertex array object and set data");
}

void i_prMeshDestroyOnGPU(prMeshData* mesh) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "Destroying mesh on GPU");

    mesh->context->DeleteVertexArrays(1, &mesh->VAO);
    mesh->context->DeleteBuffers(1, &mesh->VBO);
    mesh->context->DeleteBuffers(1, &mesh->EBO);
}
