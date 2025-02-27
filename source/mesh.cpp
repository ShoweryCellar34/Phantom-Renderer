#include <PR/mesh.hpp>

#include <PR/window.hpp>

PR::meshData::meshData() : i_windowLink(nullptr), i_vertices(nullptr), i_textureCoordinates(nullptr), i_indices(nullptr), i_verticesCount(0), i_textureCoordinatesCount(0), i_indicesCount(0), i_VBO(0), i_VAO(0), i_EBO(0) {
}

PR::meshData::~meshData() {
    if(!i_vertices) {
        delete[] i_vertices;
        delete[] i_indices;
    }
    if(!i_textureCoordinates) {
        delete[] i_textureCoordinates;
    }
}

PR::meshData::meshData(const meshData& original) : i_verticesCount(original.i_verticesCount), i_indicesCount(original.i_indicesCount), i_textureCoordinatesCount(original.i_textureCoordinatesCount) {
    if(!original.i_vertices) {
        i_vertices = new float[original.i_verticesCount];
        for(unsigned int i = 0; i < original.i_verticesCount; i++) {
            i_vertices[i] = original.i_vertices[i];
        }
    
        i_indices = new unsigned int[original.i_indicesCount];
        for(unsigned int i = 0; i < original.i_indicesCount; i++) {
            i_indices[i] = original.i_indices[i];
        }
    }

    if(original.i_textureCoordinatesCount) {
        i_textureCoordinates = new float[original.i_textureCoordinatesCount];
        for(unsigned int i = 0; i < original.i_textureCoordinatesCount; i++) {
            i_textureCoordinates[i] = original.i_textureCoordinates[i];
        }
    }
}

PR::meshData& PR::meshData::operator=(const meshData& original) {
    if(!original.i_vertices) {
        i_vertices = new float[original.i_verticesCount];
        for(unsigned int i = 0; i < original.i_verticesCount; i++) {
            i_vertices[i] = original.i_vertices[i];
        }
        i_verticesCount = original.i_verticesCount;
        
        i_indices = new unsigned int[original.i_indicesCount];
        for(unsigned int i = 0; i < original.i_indicesCount; i++) {
            i_indices[i] = original.i_indices[i];
        }
        i_indicesCount = original.i_indicesCount;
    }

    if(original.i_textureCoordinates) {
        i_textureCoordinates = new float[original.i_textureCoordinatesCount];
        for(unsigned int i = 0; i < original.i_textureCoordinatesCount; i++) {
            i_textureCoordinates[i] = original.i_textureCoordinates[i];
        }
    }
    i_textureCoordinatesCount = original.i_textureCoordinatesCount;

    return *this;
}

void PR::meshData::i_createGPUMesh() {
    i_windowLink->i_openglContext.GenVertexArrays(1, &i_VAO);
    i_windowLink->i_openglContext.GenBuffers(1, &i_VBO);
    i_windowLink->i_openglContext.GenBuffers(1, &i_EBO);
    i_windowLink->i_openglContext.BindVertexArray(i_VAO);

    i_windowLink->i_openglContext.BindBuffer(GL_ARRAY_BUFFER, i_VBO);
    i_windowLink->i_openglContext.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_EBO);

    if(i_vertices) {
        
    }

    i_windowLink->i_openglContext.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
    i_windowLink->i_openglContext.EnableVertexAttribArray(0);

    i_windowLink->i_openglContext.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    i_windowLink->i_openglContext.EnableVertexAttribArray(1);

    i_windowLink->i_openglContext.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    i_windowLink->i_openglContext.EnableVertexAttribArray(2);

    i_windowLink->i_openglContext.BindBuffer(GL_ARRAY_BUFFER, 0); 

    i_windowLink->i_openglContext.BindVertexArray(0);
}

void PR::meshData::updateMesh(float vertices[], unsigned int verticesCount, unsigned int indices[], unsigned int indicesCount, float textureCoordinates[], unsigned int textureCoordinatesCount) {
    if(!i_vertices) {
        delete[] i_vertices;
        delete[] i_indices;
    }
    if(!i_textureCoordinates) {
        delete[] i_textureCoordinates;
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

    if(textureCoordinatesCount) {
        i_textureCoordinates = new float[textureCoordinatesCount];
        for(unsigned int i = 0; i < textureCoordinatesCount; i++) {
            i_textureCoordinates[i] = textureCoordinates[i];
        }
    }
    i_textureCoordinatesCount = textureCoordinatesCount;
}

void PR::meshData::updateMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& textureCoordinates) {
    if(!i_vertices) {
        delete[] i_vertices;
        delete[] i_indices;
    }
    if(!i_textureCoordinates) {
        delete[] i_textureCoordinates;
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

    if(textureCoordinates.size()) {
        i_vertices = new float[textureCoordinates.size()];
        for(unsigned int i = 0; i < textureCoordinates.size(); i++) {
            i_vertices[i] = textureCoordinates[i];
        }
    }
    i_verticesCount = textureCoordinates.size();
}

void PR::meshData::link(window* window) {
    i_windowLink = window;
}
