#pragma once

#include <unordered_map>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace PR {
    struct meshData;

    struct window {
    public:
        GLFWwindow* i_window;
        GladGLContext i_openglContext;
        std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> i_VAOList;

        void makeWindow(std::string title, int width, int height);

        void makeContext();

        void prepMesh(meshData, std::string alias);

        void drawMesh(std::string alias);

        unsigned int genDefaultShaderProgram();
    };
}
