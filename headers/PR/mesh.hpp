#pragma once

#include <vector>

namespace PR {
    struct meshData {
    public:
        float* i_vertices = nullptr;
        unsigned int* i_indices = nullptr;
        float* i_textureCoordinates = nullptr;
        unsigned int i_verticesCount = 0, i_indicesCount = 0, i_textureCoordinatesCount = 0;

        meshData() = default;
        ~meshData();
        meshData(const meshData& original);
        meshData& operator=(const meshData& original);

        void updateMesh(float vertices[], unsigned int verticesCount, unsigned int indices[], unsigned int indicesCount, float textureCoordinates[], unsigned int textureCoordinatesCount);

        void updateMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& textureCoordinates);
    };
}
