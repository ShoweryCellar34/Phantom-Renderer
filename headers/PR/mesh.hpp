#pragma once

#include <vector>
#include <glad/gl.h>

namespace PR {
    struct meshData {
    public:
        float* i_vertices = nullptr;
        unsigned int* i_indices = nullptr;
        unsigned int i_verticesCount = 0, i_indicesCount = 0;

        meshData() = default;
        ~meshData();
        meshData(const meshData& original);
        meshData& operator=(const meshData& original);

        void updateMesh(float vertices[], unsigned int verticesCount, unsigned int indices[], unsigned int indicesCount);

        void updateMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
    };
}
