#include <setup.hpp>

#include <globals.hpp>

#include <cstdio>
#include <PR/PR.h>
#include <files.hpp>
#include <callbacks.hpp>

void setupPaths() {
    g_userDataPath = getUserDataPath() / "Example/";
    std::filesystem::create_directories(g_userDataPath);
    g_resourcesPath = getExecutablePath().parent_path();
}

void setLogFilePath(std::filesystem::path logFilePath) {
    g_logFilePath = logFilePath;
}

void setupLog() {
    prLogSetLevel(PR_LOG_TRACE);
    prLogSetStream(1, (FILE**)stdout);

    g_logFile = fopen(g_logFilePath.u8string().c_str(), "w");
    FILE* logStreams[] = {stdout, g_logFile};
    prLogSetStream(2, logStreams);
}

void shutdownLog() {
    fclose(g_logFile);
    printf("Log file saved to path: %s", g_logFilePath.u8string().c_str());
}

void setupWindow() {
    if(!g_windowInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "setupWindow: Initializing window");

        if(!glfwInit()) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupWindow: Failed to initialize GLFW. Aborting operation, nothing was modified");
            return;
        }

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        g_window = prWindowCreate(TITLE, windowWidth, windowHeight);
        if(!g_window->window) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupWindow: Failed to create window. Aborting operation, nothing was modified");
            prWindowDestroy(g_window);
            return;
        }
        prWindowInitContext(g_window);

        glfwMakeContextCurrent(g_window->window);
        glfwSetFramebufferSizeCallback(g_window->window, framebufferSizeCallback);
        glfwSetCursorPosCallback(g_window->window, cursorPosCallback);
        glfwSetKeyCallback(g_window->window, keyCallback);

        glfwSetInputMode(g_window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        g_window->openglContext->Enable(GL_DEBUG_OUTPUT);
        g_window->openglContext->Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        g_window->openglContext->DebugMessageCallback(openglDebugOutput, NULL);
        g_window->openglContext->DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

        g_windowInit = true;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupWindow: Window already initialized. Aborting operation, nothing was modified");
    }
}

void shutdownWindow() {
    if(g_windowInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownWindow: Deinitializing window");
        
        prWindowDestroy(g_window);

        glfwTerminate();

        g_windowInit = false;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownWindow: Window already deinitialized. Aborting operation, nothing was modified");
    }
}

void setupShaders() {
    prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "setupShaders: Creating shaders");

    if(!g_windowInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupShaders: Window not initialized. Aborting operation, nothing was modified");
        return;
    }

    if(g_resourcesPath.empty()) {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupShaders: Resources path not set. Aborting operation, nothing was modified");
        return;
    }

    shaderProgram = loadShader(g_window->openglContext, (g_resourcesPath / "res/shaders/defaultVertexShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/defaultFragmentShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/defaultGeometryShader.glsl").u8string().c_str());
    debugShaderProgram = loadShader(g_window->openglContext, (g_resourcesPath / "res/shaders/debugVertexShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/debugFragmentShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/debugGeometryShader.glsl").u8string().c_str());

    depthShaderProgram = loadShader(g_window->openglContext, (g_resourcesPath / "res/shaders/depthVertexShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/depthFragmentShader.glsl").u8string().c_str(), NULL);
    depth2ShaderProgram = loadShader(g_window->openglContext, (g_resourcesPath / "res/shaders/depth2VertexShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/depth2FragmentShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/depth2GeometryShader.glsl").u8string().c_str());

    skyboxShaderProgram = loadShader(g_window->openglContext, (g_resourcesPath / "res/shaders/skyboxVertexShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/skyboxFragmentShader.glsl").u8string().c_str(), NULL);

    hudShaderProgram = loadShader(g_window->openglContext, (g_resourcesPath / "res/shaders/HUDVertexShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/HUDFragmentShader.glsl").u8string().c_str(), NULL);

    gaussianShaderProgram = loadShader(g_window->openglContext, (g_resourcesPath / "res/shaders/HUDVertexShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/gaussianFragmentShader.glsl").u8string().c_str(), NULL);

    hdrShaderProgram = loadShader(g_window->openglContext, (g_resourcesPath / "res/shaders/hdrVertexShader.glsl").u8string().c_str(), (g_resourcesPath / "res/shaders/hdrFragmentShader.glsl").u8string().c_str(), NULL);

    computeShaderProgram = loadComputeShader(g_window->openglContext, (g_resourcesPath / "res/shaders/postProcessingComputeShader.glsl").u8string().c_str());
}

void shutdownShaders() {
    prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownShaders: Destroying shaders");

    if(!g_windowInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownShaders: Window not initialized. Aborting operation, nothing was modified");
        return;
    }

    prShaderDestroy(shaderProgram);
    shaderProgram = NULL;
    prShaderDestroy(debugShaderProgram);
    debugShaderProgram = NULL;
    prShaderDestroy(depthShaderProgram);
    depthShaderProgram = NULL;
    prShaderDestroy(depth2ShaderProgram);
    depth2ShaderProgram = NULL;
    prShaderDestroy(skyboxShaderProgram);
    skyboxShaderProgram = NULL;
    prShaderDestroy(hudShaderProgram);
    hudShaderProgram = NULL;
    prShaderDestroy(gaussianShaderProgram);
    gaussianShaderProgram = NULL;
    prShaderDestroy(hdrShaderProgram);
    hdrShaderProgram = NULL;
    prComputeShaderDestroy(computeShaderProgram);
    computeShaderProgram = NULL;
}
