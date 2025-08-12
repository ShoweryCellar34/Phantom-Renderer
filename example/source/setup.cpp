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
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownWindow: Window not shutdown. Aborting operation, nothing was modified");
    }
}

void setupShaders() {
    if(!g_shadersInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "setupShaders: Creating shaders");

        if(!g_windowInit) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupShaders: Window not initialized. Aborting operation, nothing was modified");
            return;
        }

        if(g_resourcesPath.empty()) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupShaders: Resources path not set. Aborting operation, nothing was modified");
            return;
        }

        shaderProgram = loadShader(g_window->openglContext, TO_RES("res/shaders/defaultVertexShader.glsl"), TO_RES("res/shaders/defaultFragmentShader.glsl"), TO_RES("res/shaders/defaultGeometryShader.glsl"));
        debugShaderProgram = loadShader(g_window->openglContext, TO_RES("res/shaders/debugVertexShader.glsl"), TO_RES("res/shaders/debugFragmentShader.glsl"), TO_RES("res/shaders/debugGeometryShader.glsl"));

        depthShaderProgram = loadShader(g_window->openglContext, TO_RES("res/shaders/depthVertexShader.glsl"), TO_RES("res/shaders/depthFragmentShader.glsl"), NULL);
        depth2ShaderProgram = loadShader(g_window->openglContext, TO_RES("res/shaders/depth2VertexShader.glsl"), TO_RES("res/shaders/depth2FragmentShader.glsl"), TO_RES("res/shaders/depth2GeometryShader.glsl"));

        skyboxShaderProgram = loadShader(g_window->openglContext, TO_RES("res/shaders/skyboxVertexShader.glsl"), TO_RES("res/shaders/skyboxFragmentShader.glsl"), NULL);

        hudShaderProgram = loadShader(g_window->openglContext, TO_RES("res/shaders/HUDVertexShader.glsl"), TO_RES("res/shaders/HUDFragmentShader.glsl"), NULL);

        gaussianShaderProgram = loadShader(g_window->openglContext, TO_RES("res/shaders/HUDVertexShader.glsl"), TO_RES("res/shaders/gaussianFragmentShader.glsl"), NULL);

        hdrShaderProgram = loadShader(g_window->openglContext, TO_RES("res/shaders/hdrVertexShader.glsl"), TO_RES("res/shaders/hdrFragmentShader.glsl"), NULL);

        computeShaderProgram = loadComputeShader(g_window->openglContext, TO_RES("res/shaders/postProcessingComputeShader.glsl"));

        g_shadersInit = true;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupShaders: Shaders already initialized. Aborting operation, nothing was modified");
    }
}

void shutdownShaders() {
    if(g_shadersInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownShaders: Destroying shaders");

        if(!g_windowInit) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownShaders: Window not initialized. Aborting operation, nothing was modified");
            return;
        }

        prShaderDestroy(shaderProgram);
        shaderProgram = nullptr;
        prShaderDestroy(debugShaderProgram);
        debugShaderProgram = nullptr;
        prShaderDestroy(depthShaderProgram);
        depthShaderProgram = nullptr;
        prShaderDestroy(depth2ShaderProgram);
        depth2ShaderProgram = nullptr;
        prShaderDestroy(skyboxShaderProgram);
        skyboxShaderProgram = nullptr;
        prShaderDestroy(hudShaderProgram);
        hudShaderProgram = nullptr;
        prShaderDestroy(gaussianShaderProgram);
        gaussianShaderProgram = nullptr;
        prShaderDestroy(hdrShaderProgram);
        hdrShaderProgram = nullptr;
        prComputeShaderDestroy(computeShaderProgram);
        computeShaderProgram = nullptr;

        g_shadersInit = false;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownShaders: Shaders not initialized. Aborting operation, nothing was modified");
    }
}

void setupFramebuffers() {
    if(!g_framebuffersInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "setupFramebuffers: Creating framebuffers");

        g_colorTextureDefault = prTextureCreate();
        prTextureLinkContext(g_colorTextureDefault, g_window->openglContext);
        prTextureUpdate(g_colorTextureDefault, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, windowWidth, windowHeight);

        g_depthStencilRBODefault = prRenderBufferCreate();
        prRenderBufferLinkContext(g_depthStencilRBODefault, g_window->openglContext);
        prRenderBufferUpdate(g_depthStencilRBODefault, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight, 0);

        g_framebufferDefault = prFramebufferCreate();
        prFramebufferLinkContext(g_framebufferDefault, g_window->openglContext);
        prFramebufferLinkColorTexture(g_framebufferDefault, g_colorTextureDefault, 0);
        prFramebufferLinkDepthStencilRBO(g_framebufferDefault, g_depthStencilRBODefault);



        g_window->openglContext->Enable(GL_MULTISAMPLE);
        g_colorRBOMultisampled = prRenderBufferCreate();
        prRenderBufferLinkContext(g_colorRBOMultisampled, g_window->openglContext);
        prRenderBufferUpdate(g_colorRBOMultisampled, PR_FORMAT_RGBA, windowWidth, windowHeight, SAMPLES);

        g_colorMultisamlpedTextureMultisampled = prTextureMultisampledCreate();
        prTextureMultisampledLinkContext(g_colorMultisamlpedTextureMultisampled, g_window->openglContext);
        prTextureMultisampledUpdate(g_colorMultisamlpedTextureMultisampled, PR_FORMAT_RGBA, windowWidth, windowHeight, SAMPLES);

        g_depthStencilMultisampledRBOMultisampled = prRenderBufferCreate();
        prRenderBufferLinkContext(g_depthStencilMultisampledRBOMultisampled, g_window->openglContext);
        prRenderBufferUpdate(g_depthStencilMultisampledRBOMultisampled, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight, SAMPLES);

        g_framebufferMultisampled = prFramebufferCreate();
        prFramebufferLinkContext(g_framebufferMultisampled, g_window->openglContext);
        prFramebufferLinkColorRBO(g_framebufferMultisampled, g_colorRBOMultisampled, 0);
        prFramebufferLinkColorTextureMultisampled(g_framebufferMultisampled, g_colorMultisamlpedTextureMultisampled, 1);
        GLenum tempAttachmentsArray[] = {PR_COLOR_ATTACHMENT_0, PR_COLOR_ATTACHMENT_1};
        prFramebufferDrawBuffers(g_framebufferMultisampled, 2, tempAttachmentsArray);
        prFramebufferLinkDepthStencilRBO(g_framebufferMultisampled, g_depthStencilMultisampledRBOMultisampled);



        g_bloomTexture = prTextureCreate();
        prTextureLinkContext(g_bloomTexture, g_window->openglContext);
        prTextureUpdate(g_bloomTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, windowWidth, windowHeight);

        g_framebufferBloom = prFramebufferCreate();
        prFramebufferLinkContext(g_framebufferBloom, g_window->openglContext);
        prFramebufferLinkColorTexture(g_framebufferBloom, g_bloomTexture, 0);



        g_colorTextureGaussian1 = prTextureCreate();
        prTextureLinkContext(g_colorTextureGaussian1, g_window->openglContext);
        prTextureUpdate(g_colorTextureGaussian1, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, windowWidth, windowHeight);

        g_framebufferGaussian1 = prFramebufferCreate();
        prFramebufferLinkContext(g_framebufferGaussian1, g_window->openglContext);
        prFramebufferLinkColorTexture(g_framebufferGaussian1, g_colorTextureGaussian1, 0);



        g_colorTextureGaussian2 = prTextureCreate();
        prTextureLinkContext(g_colorTextureGaussian2, g_window->openglContext);
        prTextureUpdate(g_colorTextureGaussian2, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, windowWidth, windowHeight);

        g_framebufferGaussian2 = prFramebufferCreate();
        prFramebufferLinkContext(g_framebufferGaussian2, g_window->openglContext);
        prFramebufferLinkColorTexture(g_framebufferGaussian2, g_colorTextureGaussian2, 0);



        gaussianFramebuffers[0] = g_framebufferGaussian1;
        gaussianFramebuffers[1] = g_framebufferGaussian2;
        gaussianTextures[0] = g_colorTextureGaussian1;
        gaussianTextures[1] = g_colorTextureGaussian2;



        g_depthTextureSunShadowMap = prTextureCreate();
        prTextureLinkContext(g_depthTextureSunShadowMap, g_window->openglContext);
        prTextureUpdate(g_depthTextureSunShadowMap, PR_FORMAT_DEPTH, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, PR_FILTER_LINEAR, false, NULL, 0, 2048, 2048);
        GLfloat tempBorderColorArray[] = {1.0f, 0.0f, 0.0f, 1.0f};
        prTextureBorderColor(g_depthTextureSunShadowMap, tempBorderColorArray);

        g_framebufferSunShadowMap = prFramebufferCreate();
        prFramebufferLinkContext(g_framebufferSunShadowMap, g_window->openglContext);
        prFramebufferLinkDepthTexture(g_framebufferSunShadowMap, g_depthTextureSunShadowMap);
        prFramebufferSetDrawBuffer(g_framebufferSunShadowMap, PR_NONE);
        prFramebufferSetReadBuffer(g_framebufferSunShadowMap, PR_NONE);

        g_depthCubeMapPointShadowMap = prCubeMapCreate();
        prCubeMapLinkContext(g_depthCubeMapPointShadowMap, g_window->openglContext);
        GLenum tempFormatsArray[6] = {PR_FORMAT_DEPTH, PR_FORMAT_DEPTH, PR_FORMAT_DEPTH, PR_FORMAT_DEPTH, PR_FORMAT_DEPTH, PR_FORMAT_DEPTH};
        GLubyte* tempTextureDataArray[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
        size_t tempTextureDataSizeArray[6] = {0, 0, 0, 0, 0, 0};
        GLsizei tempCubeMapSizeArray[6] = {1024, 1024, 1024, 1024, 1024, 1024};
        prCubeMapUpdateAll(g_depthCubeMapPointShadowMap,
            tempFormatsArray,
            PR_WRAPPING_EDGE, PR_FILTER_LINEAR, PR_FILTER_LINEAR, false,
            tempTextureDataArray,
            tempTextureDataSizeArray,
            tempCubeMapSizeArray,
            tempCubeMapSizeArray);
        prCubeMapBorderColor(g_depthCubeMapPointShadowMap, tempBorderColorArray);

        g_framebufferPointShadowMap = prFramebufferCreate();
        prFramebufferLinkContext(g_framebufferPointShadowMap, g_window->openglContext);
        prFramebufferLinkDepthCubeMap(g_framebufferPointShadowMap, g_depthCubeMapPointShadowMap);
        prFramebufferSetDrawBuffer(g_framebufferPointShadowMap, GL_NONE);
        prFramebufferSetReadBuffer(g_framebufferPointShadowMap, GL_NONE);

        g_framebuffersInit = true;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupFramebuffers: Framebuffers already initialized. Aborting operation, nothing was modified");
    }
}

void shutdownFramebuffers() {
    if(g_framebuffersInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownFramebuffers: Destroying framebuffers");

        prFramebufferDestroy(g_framebufferDefault);
        g_framebufferDefault = nullptr;
        prTextureDestroy(g_colorTextureDefault);
        g_colorTextureDefault = nullptr;
        prRenderBufferDestroy(g_depthStencilRBODefault);
        g_depthStencilRBODefault = nullptr;

        prFramebufferDestroy(g_framebufferMultisampled);
        g_framebufferMultisampled = nullptr;
        prRenderBufferDestroy(g_colorRBOMultisampled);
        g_colorRBOMultisampled = nullptr;
        prTextureMultisampledDestroy(g_colorMultisamlpedTextureMultisampled);
        g_colorMultisamlpedTextureMultisampled = nullptr;
        prRenderBufferDestroy(g_depthStencilMultisampledRBOMultisampled);
        g_depthStencilMultisampledRBOMultisampled = nullptr;

        prFramebufferDestroy(g_framebufferBloom);
        g_framebufferBloom = nullptr;
        prTextureDestroy(g_bloomTexture);
        g_bloomTexture = nullptr;

        prFramebufferDestroy(g_framebufferGaussian1);
        g_framebufferGaussian1 = nullptr;
        prTextureDestroy(g_colorTextureGaussian1);
        g_colorTextureGaussian1 = nullptr;

        prFramebufferDestroy(g_framebufferGaussian2);
        g_framebufferGaussian2 = nullptr;
        prTextureDestroy(g_colorTextureGaussian2);
        g_colorTextureGaussian2 = nullptr;

        gaussianFramebuffers[0] = nullptr;
        gaussianFramebuffers[1] = nullptr;
        gaussianTextures[0] = nullptr;
        gaussianTextures[1] = nullptr;

        prFramebufferDestroy(g_framebufferSunShadowMap);
        g_framebufferSunShadowMap = nullptr;
        prTextureDestroy(g_depthTextureSunShadowMap);
        g_depthTextureSunShadowMap = nullptr;

        prFramebufferDestroy(g_framebufferPointShadowMap);
        g_framebufferPointShadowMap = nullptr;
        prCubeMapDestroy(g_depthCubeMapPointShadowMap);
        g_depthCubeMapPointShadowMap = nullptr;

        g_framebuffersInit = false;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownFramebuffers: Framebuffers not initialized. Aborting operation, nothing was modified");
    }
}

void setupTextures() {
    if(!g_textureInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "setupTextures: Creating textures");

        defaultTexture = makeTextureCheckerboard(g_window->openglContext, 8, (float[4]){1.0f, 0.0f, 1.0f, 1.0f}, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});
        containerTexture = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, TO_RES("res/textures/container.jpg"));
        containerMetalTexture = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, TO_RES("res/textures/container2.png"));
        containerMetalSpecularTexture = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, false, TO_RES("res/textures/container2_specular.png"));
        steelTexture = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, TO_RES("res/textures/steel.jpg"));
        steelNormal = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, false, TO_RES("res/textures/steelNormal.png"));
        brickWallDiffuseTexture = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, TO_RES("res/textures/brickwall.jpg"));
        brickWallNormalTexture = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, false, TO_RES("res/textures/brickwall_normal.jpg"));
        blackTexture = makeTextureSingleColor(g_window->openglContext, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});
        whiteTexture = makeTextureSingleColor(g_window->openglContext, (float[4]){1.0f, 1.0f, 1.0f, 1.0f});
        defaultNormal = makeTextureSingleColor(g_window->openglContext, (float[4]){0.0f, -1.0f, 0.0f, 1.0f});
        HUDTexture = loadTexture(g_window->openglContext, PR_FILTER_NEAREST_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, TO_RES("res/textures/HUD.png"));
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupTextures: Textures already initialized. Aborting operation, nothing was modified");
    }
}

void shutdownTextures() {
    prTextureDestroy(defaultTexture);
    defaultTexture = nullptr;
    prTextureDestroy(containerTexture);
    containerTexture = nullptr;
    prTextureDestroy(containerMetalTexture);
    containerMetalTexture = nullptr;
    prTextureDestroy(containerMetalSpecularTexture);
    containerMetalSpecularTexture = nullptr;
    prTextureDestroy(steelTexture);
    steelTexture = nullptr;
    prTextureDestroy(steelNormal);
    steelNormal = nullptr;
    prTextureDestroy(brickWallDiffuseTexture);
    brickWallDiffuseTexture = nullptr;
    prTextureDestroy(brickWallNormalTexture);
    brickWallNormalTexture = nullptr;
    prTextureDestroy(blackTexture);
    blackTexture = nullptr;
    prTextureDestroy(whiteTexture);
    whiteTexture = nullptr;
    prTextureDestroy(defaultNormal);
    defaultNormal = nullptr;
    prTextureDestroy(HUDTexture);
    HUDTexture = nullptr;
}
