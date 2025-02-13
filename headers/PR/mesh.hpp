#pragma once

#include <vector>
#include <glad/gl.h>

namespace PR {
    struct mesh {
    public:
        GladGLContext* i_openglContext = nullptr;
        unsigned int VBO = 0, VAO = 0, EBO = 0;
        std::vector<float> i_vertices;
        std::vector<unsigned int> i_indices;
        unsigned int i_verticesCount = 0, i_indicesCount = 0;

        void updateMesh(float vertices[], unsigned int verticesCount, unsigned int indices[], unsigned int indicesCount);

        void updateMesh(std::vector<float> vertices, std::vector<unsigned int> indices);

        void buildMesh();
    };
}
