#include <PR/mesh.hpp>

void PR::mesh::updateMesh(float vertices[], unsigned int verticesCount, unsigned int indices[], unsigned int indicesCount) {
    i_vertices.assign(vertices, vertices + verticesCount);;
    i_verticesCount = verticesCount;

    i_indices.assign(indices, indices + indicesCount);;
    i_indicesCount = indicesCount;
}

void PR::mesh::updateMesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
    i_vertices = vertices;
    i_verticesCount = vertices.size();

    i_indices = indices;
    i_indicesCount = indices.size();
}

void PR::mesh::buildMesh() {
    float vertices[i_verticesCount];
    float indices[i_indicesCount];

    for(unsigned int i = 0; i < i_verticesCount; i++) {
        vertices[i] = i_vertices[i];
    }
    for(unsigned int i = 0; i < i_indicesCount; i++) {
        indices[i] = i_indices[i];
    }

    i_openglContext->GenVertexArrays(1, &VAO);
    i_openglContext->GenBuffers(1, &VBO);
    i_openglContext->GenBuffers(1, &EBO);
    i_openglContext->BindVertexArray(VAO);

    i_openglContext->BindBuffer(GL_ARRAY_BUFFER, VBO);
    i_openglContext->BufferData(GL_ARRAY_BUFFER, i_verticesCount, vertices, GL_STATIC_DRAW);

    i_openglContext->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    i_openglContext->BufferData(GL_ELEMENT_ARRAY_BUFFER, i_indicesCount, indices, GL_STATIC_DRAW);

    i_openglContext->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    i_openglContext->EnableVertexAttribArray(0);

    i_openglContext->BindBuffer(GL_ARRAY_BUFFER, 0); 

    i_openglContext->BindVertexArray(0);
}
