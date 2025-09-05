#include <callbacks.hpp>

#include <globals.hpp>

#include <thread>

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    g_windowRawWidth = width;
    g_windowRawHeight = height;
    g_windowWidth = width;
    g_windowHeight = height;
    GladGLContext* context = reinterpret_cast<GladGLContext*>(glfwGetWindowUserPointer(window));
    context->Viewport(0, 0, g_windowWidth, g_windowHeight);

    prShaderSetUniform2f(g_shaderDebug, "screenSize", g_windowWidth, g_windowHeight);

    prTextureUpdate(g_texturePostProcessing, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);

    prRenderBufferUpdate(g_RBOColorMultisampled, PR_FORMAT_RGBA, g_windowWidth, g_windowHeight, SAMPLES);
    prTextureMultisampledUpdate(g_multisampledTextureColorMultisampled, PR_FORMAT_RGBA, g_windowWidth, g_windowHeight, SAMPLES);
    prRenderBufferUpdate(g_multisampledRBODepthStencilMultisampled, PR_FORMAT_DEPTH_STENCIL, g_windowWidth, g_windowHeight, SAMPLES);

    prTextureUpdate(g_textureColorGaussianBlur1, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
    prTextureUpdate(g_textureColorGaussianBlur2, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);

    prTextureUpdate(g_textureBloom, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);

    prTextureUpdate(g_textureColorDefault, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
    prRenderBufferUpdate(g_RBODepthStencilDefault, PR_FORMAT_DEPTH_STENCIL, g_windowWidth, g_windowHeight, 0);

    prRenderBufferUpdate(g_RBOColorScreenShot, PR_FORMAT_RGB, g_windowRawWidth, g_windowRawHeight, 0);

    prCameraUpdateDimentions(g_camera);
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

    g_yaw += xoffset;
    g_pitch += yoffset;

    if(g_pitch > 89.0f) {
        g_pitch = 89.0f;
    }
    if(g_pitch < -89.0f) {
        g_pitch = -89.0f;
    }
}

void saveScreenShot(std::string path, unsigned char* pixels, int channels) {
    stbi_write_png(path.c_str(), g_windowRawWidth, g_windowRawHeight, channels, pixels, g_windowRawWidth * channels);
    prFree(pixels);

    return;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_X && action == GLFW_PRESS) {
        g_showHUD = !g_showHUD;
    }
    if(key == GLFW_KEY_Z && action == GLFW_PRESS) {
        g_showPostProcessing = !g_showPostProcessing;
    }

    if(key == GLFW_KEY_1 && action == GLFW_PRESS) {
        g_currentSkybox = 1;
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS) {
        g_currentSkybox = 2;
    }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS) {
        g_currentSkybox = 3;
    }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS) {
        g_currentSkybox = 4;
    }
    if(key == GLFW_KEY_0 && action == GLFW_PRESS) {
        g_currentSkybox = 0;
    }

    if(key == GLFW_KEY_BACKSLASH && action == GLFW_PRESS) {
        g_useDebugShader = !g_useDebugShader;
    }

    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS && (!g_screenShotThread || g_screenShotThread->joinable())) {
        GladGLContext* context = reinterpret_cast<GladGLContext*>(glfwGetWindowUserPointer(window));

        prFramebufferBlit(context, g_framebufferDefault, g_framebufferScreenShot,
            0, 0, g_windowWidth, g_windowHeight,
            0, 0, g_windowRawWidth, g_windowRawHeight,
            PR_BUFFER_BIT_COLOR, PR_FILTER_NEAREST
        );
        prFramebufferBind(g_framebufferScreenShot);

        int channels = 3;

        unsigned char* pixels = reinterpret_cast<unsigned char*>(prMalloc(g_windowRawWidth * g_windowRawHeight * channels));
        context->ReadPixels(0, 0, g_windowRawWidth, g_windowRawHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        int timestamp = time(NULL);
        char name[48];
        snprintf(name, 48, "prScreenshot%i.png", timestamp);

        stbi_flip_vertically_on_write(1);
        std::filesystem::create_directories(TO_USR("screenshots/"));
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "Saving screenshot with dimentions %ix%i to path: %s", g_windowRawWidth, g_windowRawHeight, TO_USR("screenshots" / name));

        if(g_screenShotThread == nullptr) {
            g_screenShotThread = new std::thread(saveScreenShot, std::string(TO_USR("screenshots" / name)), pixels, channels);
        } else {
            g_screenShotThread->join();
            delete g_screenShotThread;
            g_screenShotThread = new std::thread(saveScreenShot, std::string(TO_USR("screenshots" / name)), pixels, channels);
        }
    }

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void cubeMeshTranslationUpdate(prMeshData* mesh, void* translation) {
    if(translation) {
        prShaderSetUniformMatrix4fv(g_currentShaderProgram, "translation", (GLfloat*)translation);
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
