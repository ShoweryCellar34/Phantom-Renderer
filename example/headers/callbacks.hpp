#pragma once

#include <PR/PR.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void APIENTRY openglDebugOutput(GLenum source, 
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char* message,
                            const void* userParam
);
