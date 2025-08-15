#include <callbacks.hpp>

#include <globals.hpp>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    g_windowWidth = width;
    g_windowHeight = height;
    GladGLContext* context = reinterpret_cast<GladGLContext*>(glfwGetWindowUserPointer(window));
    context->Viewport(0, 0, g_windowWidth, g_windowHeight);

    prShaderSetUniform2f(debugShaderProgram, "screenSize", g_windowWidth, g_windowHeight);

    prTextureUpdate(g_texturePostProcessing, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);

    prRenderBufferUpdate(g_colorRBOMultisampled, PR_FORMAT_RGBA, g_windowWidth, g_windowHeight, SAMPLES);
    prTextureMultisampledUpdate(g_colorMultisamlpedTextureMultisampled, PR_FORMAT_RGBA, g_windowWidth, g_windowHeight, SAMPLES);
    prRenderBufferUpdate(g_depthStencilMultisampledRBOMultisampled, PR_FORMAT_DEPTH_STENCIL, g_windowWidth, g_windowHeight, SAMPLES);

    prTextureUpdate(g_colorTextureGaussian1, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
    prTextureUpdate(g_colorTextureGaussian2, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);

    prTextureUpdate(g_bloomTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);

    prTextureUpdate(g_colorTextureDefault, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
    prRenderBufferUpdate(g_depthStencilRBODefault, PR_FORMAT_DEPTH_STENCIL, g_windowWidth, g_windowHeight, 0);

    prCameraUpdateDimentions(camera);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouseMovement = true;
    static double lastX = 0.0f;
    static double lastY = 0.0f;

    if(firstMouseMovement) {
        lastX = xpos;
        lastY = ypos;
        firstMouseMovement = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) {
        pitch = 89.0f;
    }
    if(pitch < -89.0f) {
        pitch = -89.0f;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_X && action == GLFW_PRESS) {
        showHUD = !showHUD;
    }
    if(key == GLFW_KEY_Z && action == GLFW_PRESS) {
        showPostProcessing = !showPostProcessing;
    }

    if(key == GLFW_KEY_1 && action == GLFW_PRESS) {
        currentSkybox = 1;
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS) {
        currentSkybox = 2;
    }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS) {
        currentSkybox = 3;
    }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS) {
        currentSkybox = 4;
    }
    if(key == GLFW_KEY_0 && action == GLFW_PRESS) {
        currentSkybox = 0;
    }

    if(key == GLFW_KEY_BACKSLASH && action == GLFW_PRESS) {
        useDebugShader = !useDebugShader;
    }

    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        GladGLContext* context = reinterpret_cast<GladGLContext*>(glfwGetWindowUserPointer(window));

        GLint viewportSize[4];
        context->GetIntegerv(GL_VIEWPORT, viewportSize);
        int width = viewportSize[2];
        int height = viewportSize[3];

        unsigned char* pixels = reinterpret_cast<unsigned char*>(prMalloc(width * height * 4));
        context->ReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        int timestamp = time(NULL);
        char name[32];
        snprintf(name, 32, "%i.png", timestamp);

        stbi_flip_vertically_on_write(1);
        stbi_write_png(name, width, height, 4, pixels, width * 4);
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "Save screenshot with dimentions %ix%i and name: %s", width, height, name);

        prFree(pixels);
    }

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void APIENTRY openglDebugOutput(GLenum source, 
                                GLenum type,
                                unsigned int id,
                                GLenum severity,
                                GLsizei length,
                                const char* message,
                                const void* userParam
) {
    prLogRaw("---------------\n");
    prLogRaw("Debug message (%i): %s\n", id, message);

    switch(source) {
        case GL_DEBUG_SOURCE_API:
            prLogRaw("Source: API\n");
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            prLogRaw("Source: Window System\n");
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            prLogRaw("Source: Shader Compiler\n");
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            prLogRaw("Source: Third Party\n");
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            prLogRaw("Source: Application\n");
            break;

        case GL_DEBUG_SOURCE_OTHER:
            prLogRaw("Source: Other\n");
            break;
    }

    switch(type) {
        case GL_DEBUG_TYPE_ERROR:
            prLogRaw("Type: Error\n");
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            prLogRaw("Type: Deprecated Behaviour\n");
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            prLogRaw("Type: Undefined Behaviour\n");
            break;
 
        case GL_DEBUG_TYPE_PORTABILITY:
            prLogRaw("Type: Portability\n");
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            prLogRaw("Type: Performance\n");
            break;

        case GL_DEBUG_TYPE_MARKER:
            prLogRaw("Type: Marker\n");
            break;

        case GL_DEBUG_TYPE_PUSH_GROUP:
            prLogRaw("Type: Push Group\n");
            break;

        case GL_DEBUG_TYPE_POP_GROUP:
            prLogRaw("Type: Pop Group\n");
            break;

        case GL_DEBUG_TYPE_OTHER:
            prLogRaw("Type: Other\n");
            break;
    }
    
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            prLogRaw("Severity: high\n");
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            prLogRaw("Severity: medium\n");
            break;

        case GL_DEBUG_SEVERITY_LOW:
            prLogRaw("Severity: low\n");
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            prLogRaw("Severity: notification\n");
            break;
    }
    prLogRaw("\n");
}
