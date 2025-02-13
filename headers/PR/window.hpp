#pragma once

#include <vector>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace PR {
    struct window {
    public:
        GLFWwindow* i_window;
        GladGLContext i_openglContext;

        void makeWindow(std::string title, int width, int height);

        void makeContext();

        unsigned int prepMesh(float vertices[], unsigned int verticesCount, unsigned int indices[], unsigned int indicesCount);

        unsigned int genDefaultShaderProgram();
    };
}
