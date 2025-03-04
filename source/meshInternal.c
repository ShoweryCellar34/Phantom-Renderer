#include <PR/meshInternal.h>

#include <glad/gl.h>
#include <PR/error.h>

void i_prMeshCreateOnGPUSide(prMeshData* mesh) {
        mesh->context->GenVertexArrays(1, &mesh->VAO);
        if(!mesh->VAO) {
            prError(PR_GL_ERROR, "Failed to create vertex array object. Aborting operation, nothing was modified");
            return;
        } else {
            prLogTrace("[GL]", "Successfully created vertex array object");
        }

        mesh->context->GenBuffers(1, &mesh->VBO);
        if(!mesh->VBO) {
            mesh->context->DeleteVertexArrays(1, &mesh->VAO);
            prError(PR_GL_ERROR, "Failed to create vertex buffer object. Aborting operation, nothing was modified");
            return;
        } else {
            prLogTrace("[GL]", "Successfully created vertex buffer object");
        }

        mesh->context->GenBuffers(1, &mesh->EBO);
        if(!mesh->EBO) {
            mesh->context->DeleteVertexArrays(1, &mesh->VAO);
            mesh->context->DeleteBuffers(1, &mesh->VBO);
            prError(PR_GL_ERROR, "Failed to create index buffer object. Aborting operation, nothing was modified");
            return;
        } else {
            prLogTrace("[GL]", "Successfully created index buffer object");
        }

        mesh->context->BindVertexArray(mesh->VAO);

        mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount * sizeof(GLfloat), mesh->GPUReadyBuffer, GL_STATIC_DRAW);

        mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);

        mesh->context->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
        mesh->context->EnableVertexAttribArray(0);

        mesh->context->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        mesh->context->EnableVertexAttribArray(1);

        mesh->context->BindBuffer(GL_ARRAY_BUFFER, 0);

        mesh->context->BindVertexArray(0);
}

void i_prMeshUpdateOnGPUSide(prMeshData* mesh) {
	mesh->context->BindVertexArray(mesh->VAO);

	mesh->context->BindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	mesh->context->BufferData(GL_ARRAY_BUFFER, mesh->GPUReadyBufferCount, mesh->GPUReadyBuffer, GL_STATIC_DRAW);

	mesh->context->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
	mesh->context->BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesCount, mesh->indices, GL_STATIC_DRAW);

	mesh->context->BindVertexArray(0);
}

void i_prMeshDestroyOnGPUSide(prMeshData* mesh) {
    mesh->context->DeleteVertexArrays(1, &mesh->VAO);
    mesh->context->DeleteBuffers(1, &mesh->VBO);
    mesh->context->DeleteBuffers(1, &mesh->EBO);
}
