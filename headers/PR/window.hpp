#pragma once

#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace PR {
    struct window {
    public:
        GLFWwindow* windowInternal;
        GladGLContext openglContext;

        void makeContext();
    };
}
