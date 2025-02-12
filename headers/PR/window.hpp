#pragma once

#include <vector>
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace PR {
    struct window {
    public:
        unsigned int testTriShaderProgram = 0;
        unsigned int testTriVAO = 0;

        GLFWwindow* windowInternal;
        GladGLContext openglContext;

        void makeTestTri();

        void makeWindow(std::string title, int width, int height);

        void makeContext();

        unsigned int prepMesh(float vertices[], unsigned int indices[]);

        unsigned int genDefaultSHaderProgram();
    };
}
