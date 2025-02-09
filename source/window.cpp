#include <PR/window.hpp>

void PR::window::makeContext() {
    glfwMakeContextCurrent(windowInternal);
    gladLoadGLContext(&openglContext, (GLADloadfunc)glfwGetProcAddress);
}
