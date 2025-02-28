#include <PR/mesh.h>

#include <memory.h>

// void PR::meshData::i_createGPUMesh() {
//     i_windowLink->i_openglContext.GenVertexArrays(1, &i_VAO);
//     i_windowLink->i_openglContext.GenBuffers(1, &i_VBO);
//     i_windowLink->i_openglContext.GenBuffers(1, &i_EBO);
//     i_windowLink->i_openglContext.BindVertexArray(i_VAO);

//     i_windowLink->i_openglContext.BindBuffer(GL_ARRAY_BUFFER, i_VBO);
//     i_windowLink->i_openglContext.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_EBO);

//     if(i_vertices) {
        
//     }

//     i_windowLink->i_openglContext.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
//     i_windowLink->i_openglContext.EnableVertexAttribArray(0);

//     i_windowLink->i_openglContext.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
//     i_windowLink->i_openglContext.EnableVertexAttribArray(1);

//     i_windowLink->i_openglContext.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
//     i_windowLink->i_openglContext.EnableVertexAttribArray(2);

//     i_windowLink->i_openglContext.BindBuffer(GL_ARRAY_BUFFER, 0); 

//     i_windowLink->i_openglContext.BindVertexArray(0);
// }

void prInitMesh(prMeshData* mesh) {
    mesh->vertices = NULL;
    mesh->textureCoordinates = NULL;
    mesh->indices = NULL;
    mesh->verticesCount = 0;
    mesh->textureCoordinatesCount = 0;
    mesh->indicesCount = 0;
    mesh->VBO = 0;
    mesh->VAO = 0;
    mesh->EBO = 0;
    mesh->GPUArray = NULL;
    mesh->GPUArrayCount = 0;
}

void prUpdateMesh(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, GLuint indices[], size_t indicesCount, GLfloat textureCoordinates[], size_t textureCoordinatesCount) {
    if(mesh->vertices) {
        free(mesh->vertices);
        free(mesh->indices);
    }
    if(mesh->textureCoordinates) {
        free(mesh->textureCoordinates);
    }

    mesh->vertices = malloc(sizeof(GLfloat) * verticesCount);
    memcpy(mesh->vertices, vertices, sizeof(GLfloat) * verticesCount);
    mesh->verticesCount = verticesCount;

    mesh->indices = malloc(sizeof(GLuint) * indicesCount);
    memcpy(mesh->indices, indices, sizeof(GLuint) * indicesCount);
    mesh->indicesCount = indicesCount;

    if(textureCoordinatesCount && textureCoordinates) {
        mesh->textureCoordinates = malloc(sizeof(GLfloat) * textureCoordinatesCount);
        memcpy(mesh->textureCoordinates, textureCoordinates, sizeof(GLfloat) * textureCoordinatesCount);
        mesh->textureCoordinatesCount = textureCoordinatesCount;
    
    }
    mesh->textureCoordinatesCount = textureCoordinatesCount;

    if(mesh->GPUArray) {
        free(mesh->GPUArray);
    }

    mesh->GPUArray = malloc(sizeof(GLfloat) * 5 * verticesCount);
    if(textureCoordinates) {
        for(size_t i = 0; i < (verticesCount / 3); i++) {
            mesh->GPUArray[i] = vertices[i++];
            mesh->GPUArray[i] = vertices[i++];
            mesh->GPUArray[i] = vertices[i++];
            mesh->GPUArray[i] = textureCoordinates[i++];
            mesh->GPUArray[i] = textureCoordinates[i];
        }
    }
}
