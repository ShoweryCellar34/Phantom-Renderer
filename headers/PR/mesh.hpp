#pragma once

#include <vector>

namespace PR {
    struct window;

    struct meshData {
    public:
        window* i_windowLink;
        float* i_vertices;
        float* i_textureCoordinates;
        unsigned int* i_indices;
        unsigned int i_verticesCount, i_textureCoordinatesCount, i_indicesCount;
        unsigned int i_VBO, i_VAO, i_EBO;

        meshData();
        ~meshData();
        meshData(const meshData& original);
        meshData& operator=(const meshData& original);

        void i_createGPUMesh();

        void i_deleteGPUMesh();

        void updateMesh(float vertices[], unsigned int verticesCount, unsigned int indices[], unsigned int indicesCount, float textureCoordinates[], unsigned int textureCoordinatesCount);

        void updateMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& textureCoordinates);

        void link(window* window);
    };
}
