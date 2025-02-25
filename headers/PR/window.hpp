#pragma once

#include <unordered_map>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace PR {
    struct meshData;
    struct textureData;

    struct window {
    public:
        GLFWwindow* i_window;
        GladGLContext i_openglContext;
        std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> i_VAOList;
        std::unordered_map<std::string, unsigned int> i_TBOList;

        void makeWindow(const std::string& title, int width, int height);

        void makeContext();

        void prepMesh(const meshData& mesh, const std::string& alias);

        void prepTexture(const textureData& texture, const std::string& alias);

        void drawMesh(unsigned int shaderProgram, const std::string& meshAlias, const std::string& textureAlias);

        unsigned int genDefaultShaderProgram();
    };
}
