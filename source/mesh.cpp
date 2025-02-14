#include <PR/mesh.hpp>

PR::meshData::~meshData() {
    if(!i_vertices) {
        delete[] i_vertices;
        delete[] i_indices;
    }
}

PR::meshData::meshData(const meshData& original) : i_verticesCount(original.i_verticesCount), i_indicesCount(original.i_indicesCount) {
    if(!original.i_vertices) {
        return;
    }

    i_vertices = new float[original.i_verticesCount];
    for(unsigned int i = 0; i < original.i_verticesCount; i++) {
        i_vertices[i] = original.i_vertices[i];
    }

    i_indices = new unsigned int[original.i_indicesCount];
    for(unsigned int i = 0; i < original.i_indicesCount; i++) {
        i_indices[i] = original.i_indices[i];
    }
}

PR::meshData& PR::meshData::operator=(const meshData& original) {
    i_verticesCount = original.i_verticesCount;
    i_indicesCount = original.i_indicesCount;

    if(!original.i_vertices) {
        return *this;
    }

    i_vertices = new float[original.i_verticesCount];
    for(unsigned int i = 0; i < original.i_verticesCount; i++) {
        i_vertices[i] = original.i_vertices[i];
    }

    i_indices = new unsigned int[original.i_indicesCount];
    for(unsigned int i = 0; i < original.i_indicesCount; i++) {
        i_indices[i] = original.i_indices[i];
    }

    return *this;
}

void PR::meshData::updateMesh(float vertices[], unsigned int verticesCount, unsigned int indices[], unsigned int indicesCount) {
    if(!i_vertices) {
        delete[] i_vertices;
        delete[] i_indices;
    }

    i_vertices = new float[verticesCount];
    for(unsigned int i = 0; i < verticesCount; i++) {
        i_vertices[i] = vertices[i];
    }
    i_verticesCount = verticesCount;

    i_indices = new unsigned int[indicesCount];
    for(unsigned int i = 0; i < indicesCount; i++) {
        i_indices[i] = indices[i];
    }
    i_indicesCount = indicesCount;
}

void PR::meshData::updateMesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
    if(!i_vertices) {
        delete[] i_vertices;
        delete[] i_indices;
    }

    i_vertices = new float[vertices.size()];
    for(unsigned int i = 0; i < vertices.size(); i++) {
        i_vertices[i] = vertices[i];
    }
    i_verticesCount = vertices.size();

    i_indices = new unsigned int[indices.size()];
    for(unsigned int i = 0; i < indices.size(); i++) {
        i_indices[i] = indices[i];
    }
    i_indicesCount = indices.size();
}
