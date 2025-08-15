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
        g_window = prWindowCreate(TITLE, g_windowWidth, g_windowHeight);
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

        g_shaderDefault = loadShader(g_window->openglContext, TO_RES("res/shaders/defaultVertexShader.glsl"), TO_RES("res/shaders/defaultFragmentShader.glsl"), TO_RES("res/shaders/defaultGeometryShader.glsl"));
        g_shaderDebug = loadShader(g_window->openglContext, TO_RES("res/shaders/debugVertexShader.glsl"), TO_RES("res/shaders/debugFragmentShader.glsl"), TO_RES("res/shaders/debugGeometryShader.glsl"));

        g_shaderDirectionalLight = loadShader(g_window->openglContext, TO_RES("res/shaders/depthVertexShader.glsl"), TO_RES("res/shaders/depthFragmentShader.glsl"), NULL);
        g_shaderPointLight = loadShader(g_window->openglContext, TO_RES("res/shaders/depth2VertexShader.glsl"), TO_RES("res/shaders/depth2FragmentShader.glsl"), TO_RES("res/shaders/depth2GeometryShader.glsl"));

        g_shaderSkybox = loadShader(g_window->openglContext, TO_RES("res/shaders/skyboxVertexShader.glsl"), TO_RES("res/shaders/skyboxFragmentShader.glsl"), NULL);

        g_shaderHUD = loadShader(g_window->openglContext, TO_RES("res/shaders/HUDVertexShader.glsl"), TO_RES("res/shaders/HUDFragmentShader.glsl"), NULL);

        g_shaderGaussianBlur = loadShader(g_window->openglContext, TO_RES("res/shaders/HUDVertexShader.glsl"), TO_RES("res/shaders/gaussianFragmentShader.glsl"), NULL);

        g_shaderHDR = loadShader(g_window->openglContext, TO_RES("res/shaders/hdrVertexShader.glsl"), TO_RES("res/shaders/hdrFragmentShader.glsl"), NULL);

        g_computeShaderPostProcessing = loadComputeShader(g_window->openglContext, TO_RES("res/shaders/postProcessingComputeShader.glsl"));
        g_texturePostProcessing = prTextureCreate();
        prTextureUpdate(g_texturePostProcessing, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
        prTextureLinkContext(g_texturePostProcessing, g_window->openglContext);

        g_shadersInit = true;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupShaders: Shaders already initialized. Aborting operation, nothing was modified");
    }
}

void shutdownShaders() {
    if(g_shadersInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownShaders: Destroying shaders");

        prShaderDestroy(g_shaderDefault);
        g_shaderDefault = nullptr;
        prShaderDestroy(g_shaderDebug);
        g_shaderDebug = nullptr;
        prShaderDestroy(g_shaderDirectionalLight);
        g_shaderDirectionalLight = nullptr;
        prShaderDestroy(g_shaderPointLight);
        g_shaderPointLight = nullptr;
        prShaderDestroy(g_shaderSkybox);
        g_shaderSkybox = nullptr;
        prShaderDestroy(g_shaderHUD);
        g_shaderHUD = nullptr;
        prShaderDestroy(g_shaderGaussianBlur);
        g_shaderGaussianBlur = nullptr;
        prShaderDestroy(g_shaderHDR);
        g_shaderHDR = nullptr;
        prComputeShaderDestroy(g_computeShaderPostProcessing);
        g_computeShaderPostProcessing = nullptr;
        prTextureDestroy(g_texturePostProcessing);
        g_texturePostProcessing = nullptr;

        g_shadersInit = false;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownShaders: Shaders not initialized. Aborting operation, nothing was modified");
    }
}

void setupFramebuffers() {
    if(!g_framebuffersInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "setupFramebuffers: Creating framebuffers");

        if(!g_windowInit) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupFramebuffers: Window not initialized. Aborting operation, nothing was modified");
            return;
        }

        g_colorTextureDefault = prTextureCreate();
        prTextureUpdate(g_colorTextureDefault, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
        prTextureLinkContext(g_colorTextureDefault, g_window->openglContext);

        g_depthStencilRBODefault = prRenderBufferCreate();
        prRenderBufferUpdate(g_depthStencilRBODefault, PR_FORMAT_DEPTH_STENCIL, g_windowWidth, g_windowHeight, 0);
        prRenderBufferLinkContext(g_depthStencilRBODefault, g_window->openglContext);

        g_framebufferDefault = prFramebufferCreate();
        prFramebufferLinkColorTexture(g_framebufferDefault, g_colorTextureDefault, 0);
        prFramebufferLinkDepthStencilRBO(g_framebufferDefault, g_depthStencilRBODefault);
        prFramebufferLinkContext(g_framebufferDefault, g_window->openglContext);



        g_window->openglContext->Enable(GL_MULTISAMPLE);
        g_colorRBOMultisampled = prRenderBufferCreate();
        prRenderBufferUpdate(g_colorRBOMultisampled, PR_FORMAT_RGBA, g_windowWidth, g_windowHeight, SAMPLES);
        prRenderBufferLinkContext(g_colorRBOMultisampled, g_window->openglContext);

        g_colorMultisamlpedTextureMultisampled = prTextureMultisampledCreate();
        prTextureMultisampledUpdate(g_colorMultisamlpedTextureMultisampled, PR_FORMAT_RGBA, g_windowWidth, g_windowHeight, SAMPLES);
        prTextureMultisampledLinkContext(g_colorMultisamlpedTextureMultisampled, g_window->openglContext);

        g_depthStencilMultisampledRBOMultisampled = prRenderBufferCreate();
        prRenderBufferUpdate(g_depthStencilMultisampledRBOMultisampled, PR_FORMAT_DEPTH_STENCIL, g_windowWidth, g_windowHeight, SAMPLES);
        prRenderBufferLinkContext(g_depthStencilMultisampledRBOMultisampled, g_window->openglContext);

        g_framebufferMultisampled = prFramebufferCreate();
        prFramebufferLinkColorRBO(g_framebufferMultisampled, g_colorRBOMultisampled, 0);
        prFramebufferLinkColorTextureMultisampled(g_framebufferMultisampled, g_colorMultisamlpedTextureMultisampled, 1);
        prFramebufferLinkContext(g_framebufferMultisampled, g_window->openglContext);
        GLenum tempAttachmentsArray[] = {PR_COLOR_ATTACHMENT_0, PR_COLOR_ATTACHMENT_1};
        prFramebufferDrawBuffers(g_framebufferMultisampled, 2, tempAttachmentsArray);
        prFramebufferLinkDepthStencilRBO(g_framebufferMultisampled, g_depthStencilMultisampledRBOMultisampled);



        g_bloomTexture = prTextureCreate();
        prTextureUpdate(g_bloomTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
        prTextureLinkContext(g_bloomTexture, g_window->openglContext);

        g_framebufferBloom = prFramebufferCreate();
        prFramebufferLinkColorTexture(g_framebufferBloom, g_bloomTexture, 0);
        prFramebufferLinkContext(g_framebufferBloom, g_window->openglContext);



        g_colorTextureGaussianBlur1 = prTextureCreate();
        prTextureUpdate(g_colorTextureGaussianBlur1, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
        prTextureLinkContext(g_colorTextureGaussianBlur1, g_window->openglContext);

        g_framebufferGaussianBlur1 = prFramebufferCreate();
        prFramebufferLinkColorTexture(g_framebufferGaussianBlur1, g_colorTextureGaussianBlur1, 0);
        prFramebufferLinkContext(g_framebufferGaussianBlur1, g_window->openglContext);



        g_colorTextureGaussianBlur2 = prTextureCreate();
        prTextureUpdate(g_colorTextureGaussianBlur2, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, NULL, 0, g_windowWidth, g_windowHeight);
        prTextureLinkContext(g_colorTextureGaussianBlur2, g_window->openglContext);

        g_framebufferGaussianBlur2 = prFramebufferCreate();
        prFramebufferLinkColorTexture(g_framebufferGaussianBlur2, g_colorTextureGaussianBlur2, 0);
        prFramebufferLinkContext(g_framebufferGaussianBlur2, g_window->openglContext);



        g_gaussianBlurFramebuffers[0] = g_framebufferGaussianBlur1;
        g_gaussianBlurFramebuffers[1] = g_framebufferGaussianBlur2;
        g_gaussianBlurTextures[0] = g_colorTextureGaussianBlur1;
        g_gaussianBlurTextures[1] = g_colorTextureGaussianBlur2;



        g_depthTextureSunShadowMap = prTextureCreate();
        prTextureUpdate(g_depthTextureSunShadowMap, PR_FORMAT_DEPTH, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, PR_FILTER_LINEAR, false, NULL, 0, 2048, 2048);
        prTextureBorderColor(g_depthTextureSunShadowMap, TEMP_RGBA(1.0f, 0.0f, 0.0f, 1.0f));
        prTextureLinkContext(g_depthTextureSunShadowMap, g_window->openglContext);

        g_framebufferSunShadowMap = prFramebufferCreate();
        prFramebufferLinkDepthTexture(g_framebufferSunShadowMap, g_depthTextureSunShadowMap);
        prFramebufferSetDrawBuffer(g_framebufferSunShadowMap, PR_NONE);
        prFramebufferSetReadBuffer(g_framebufferSunShadowMap, PR_NONE);
        prFramebufferLinkContext(g_framebufferSunShadowMap, g_window->openglContext);

        g_depthCubeMapPointShadowMap = prCubeMapCreate();
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
        prCubeMapBorderColor(g_depthCubeMapPointShadowMap, TEMP_RGBA(1.0f, 0.0f, 0.0f, 1.0f));
        prCubeMapLinkContext(g_depthCubeMapPointShadowMap, g_window->openglContext);

        g_framebufferPointShadowMap = prFramebufferCreate();
        prFramebufferLinkDepthCubeMap(g_framebufferPointShadowMap, g_depthCubeMapPointShadowMap);
        prFramebufferSetDrawBuffer(g_framebufferPointShadowMap, GL_NONE);
        prFramebufferSetReadBuffer(g_framebufferPointShadowMap, GL_NONE);
        prFramebufferLinkContext(g_framebufferPointShadowMap, g_window->openglContext);

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

        prFramebufferDestroy(g_framebufferGaussianBlur1);
        g_framebufferGaussianBlur1 = nullptr;
        prTextureDestroy(g_colorTextureGaussianBlur1);
        g_colorTextureGaussianBlur1 = nullptr;

        prFramebufferDestroy(g_framebufferGaussianBlur2);
        g_framebufferGaussianBlur2 = nullptr;
        prTextureDestroy(g_colorTextureGaussianBlur2);
        g_colorTextureGaussianBlur2 = nullptr;

        g_gaussianBlurFramebuffers[0] = nullptr;
        g_gaussianBlurFramebuffers[1] = nullptr;
        g_gaussianBlurTextures[0] = nullptr;
        g_gaussianBlurTextures[1] = nullptr;

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
    if(!g_texturesInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "setupTextures: Creating textures");

        if(!g_windowInit) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupTextures: Window not initialized. Aborting operation, nothing was modified");
            return;
        }

        if(g_resourcesPath.empty()) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupTextures: Resources path not set. Aborting operation, nothing was modified");
            return;
        }

        stbi_set_flip_vertically_on_load(1);

        g_textureCheckerboard = makeTextureCheckerboard(g_window->openglContext, 8, TEMP_RGBA(1.0f, 0.0f, 1.0f, 1.0f), TEMP_RGBA(0.0f, 0.0f, 0.0f, 1.0f));
        g_textureBlack = makeTextureSingleColor(g_window->openglContext, TEMP_RGBA(0.0f, 0.0f, 0.0f, 1.0f));
        g_textureWhite = makeTextureSingleColor(g_window->openglContext, TEMP_RGBA(1.0f, 1.0f, 1.0f, 1.0f));
        g_textureNormalDefault = makeTextureSingleColor(g_window->openglContext, TEMP_RGBA(0.0f, 0.0f, 1.0f, 1.0f));

        g_textureHUD = loadTexture(g_window->openglContext, PR_FILTER_NEAREST_MIPMAP_NEAREST, PR_FILTER_LINEAR, TO_RES("res/textures/HUD.png"));

        g_textureContainer = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, TO_RES("res/textures/container.jpg"));
        g_textureMetalRimmedContainer = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, TO_RES("res/textures/container2.png"));
        g_textureMetalRimmedContainerSpecular = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, TO_RES("res/textures/container2_specular.png"));
        g_textureSteel = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, TO_RES("res/textures/steel.jpg"));
        g_textureSteelNormal = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, TO_RES("res/textures/steelNormal.png"));
        g_textureBrickWall = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, TO_RES("res/textures/brickwall.jpg"));
        g_textureBrickWallNormal = loadTexture(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, TO_RES("res/textures/brickwall_normal.jpg"));

        skyboxDefaultCubeMap = makeCubeMapSingleColors(g_window->openglContext, TEMP_RGBA_6(
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ));

        static const char* cubeMapIslandsTextures[6] = {
            "res/skyboxes/1/right.jpg",
            "res/skyboxes/1/left.jpg",
            "res/skyboxes/1/top.jpg",
            "res/skyboxes/1/bottom.jpg",
            "res/skyboxes/1/front.jpg",
            "res/skyboxes/1/back.jpg",
        };
        static const char* cubeMapSpaceTextures[6] = {
            "res/skyboxes/2/px.png",
            "res/skyboxes/2/nx.png",
            "res/skyboxes/2/py.png",
            "res/skyboxes/2/ny.png",
            "res/skyboxes/2/pz.png",
            "res/skyboxes/2/nz.png",
        };

        stbi_set_flip_vertically_on_load(0);
        g_cubeMapIslands = loadCubeMap(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_LINEAR, PR_FILTER_LINEAR, cubeMapIslandsTextures);
        g_cubeMapSpace = loadCubeMap(g_window->openglContext, PR_FILTER_LINEAR_MIPMAP_LINEAR, PR_FILTER_LINEAR, cubeMapSpaceTextures);
        stbi_set_flip_vertically_on_load(1);

        g_cubeMap3 = makeCubeMapSingleColors(g_window->openglContext, TEMP_RGBA_6(
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f
        ));

        g_cubeMap4 = makeCubeMapCheckerboards(g_window->openglContext, 32, TEMP_RGBA_6(
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f
        ), TEMP_RGBA_6(
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        ));

        g_texturesInit = true;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupTextures: Textures already initialized. Aborting operation, nothing was modified");
    }
}

void shutdownTextures() {
    if(g_texturesInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownTextures: Destroying textures");

        prTextureDestroy(g_textureCheckerboard);
        g_textureCheckerboard = nullptr;
        prTextureDestroy(g_textureBlack);
        g_textureBlack = nullptr;
        prTextureDestroy(g_textureWhite);
        g_textureWhite = nullptr;
        prTextureDestroy(g_textureNormalDefault);
        g_textureNormalDefault = nullptr;

        prTextureDestroy(g_textureHUD);
        g_textureHUD = nullptr;

        prTextureDestroy(g_textureContainer);
        g_textureContainer = nullptr;
        prTextureDestroy(g_textureMetalRimmedContainer);
        g_textureMetalRimmedContainer = nullptr;
        prTextureDestroy(g_textureMetalRimmedContainerSpecular);
        g_textureMetalRimmedContainerSpecular = nullptr;
        prTextureDestroy(g_textureSteel);
        g_textureSteel = nullptr;
        prTextureDestroy(g_textureSteelNormal);
        g_textureSteelNormal = nullptr;
        prTextureDestroy(g_textureBrickWall);
        g_textureBrickWall = nullptr;
        prTextureDestroy(g_textureBrickWallNormal);
        g_textureBrickWallNormal = nullptr;

        prCubeMapDestroy(skyboxDefaultCubeMap);
        skyboxDefaultCubeMap = nullptr;
        prCubeMapDestroy(g_cubeMapIslands);
        g_cubeMapIslands = nullptr;
        prCubeMapDestroy(g_cubeMapSpace);
        g_cubeMapSpace = nullptr;
        prCubeMapDestroy(g_cubeMap3);
        g_cubeMap3 = nullptr;
        prCubeMapDestroy(g_cubeMap4);
        g_cubeMap4 = nullptr;

        g_texturesInit = false;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownTextures: Textures not initialized. Aborting operation, nothing was modified");
    }
}

void setupMaterials() {
    if(!g_materialsInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "setupMaterials: Creating materials");

        if(!g_windowInit) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupMaterials: Window not initialized. Aborting operation, nothing was modified");
            return;
        }
        if(!g_shadersInit) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupMaterials: Shaders not initialized. Aborting operation, nothing was modified");
            return;
        }
        if(!g_texturesInit) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupMaterials: Textures not initialized. Aborting operation, nothing was modified");
            return;
        }

        g_materialCheckerboard.setMaps(g_textureCheckerboard, g_textureCheckerboard, g_textureNormalDefault, 0.0f);
        g_materialBlack.setMaps(g_textureBlack, g_textureWhite, g_textureNormalDefault, 32.0f);
        g_materialWhite.setMaps(g_textureWhite, g_textureWhite, g_textureNormalDefault, 32.0f);

        g_materialHUD.setMaps(g_textureHUD, g_textureBlack, g_textureNormalDefault, 0.0f);
        g_materialPostProcessing.setMaps(g_texturePostProcessing, g_textureBlack, g_textureNormalDefault, 0.0f);

        g_materialContainer.setMaps(g_textureContainer, g_textureBlack, g_textureNormalDefault, 0.0f);
        g_materialMetalRimmedContainer.setMaps(g_textureMetalRimmedContainer, g_textureMetalRimmedContainerSpecular, g_textureNormalDefault, 64.0f);
        g_materialSteel.setMaps(g_textureSteel, g_textureBlack, g_textureSteelNormal, 48.0f);
        g_materialBrickWall.setMaps(g_textureBrickWall, g_textureBlack, g_textureBrickWallNormal, 16.0f);

        g_materialsInit = true;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupMaterials: Materials already initialized. Aborting operation, nothing was modified");
    }
}

void shutdownMaterials() {
    if(g_materialsInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownMaterials: Destroying materials");

        g_materialCheckerboard.setMaps(nullptr, nullptr, nullptr, 0.0f);
        g_materialBlack.setMaps(nullptr, nullptr, nullptr, 0.0f);
        g_materialWhite.setMaps(nullptr, nullptr, nullptr, 0.0f);

        g_materialHUD.setMaps(nullptr, nullptr, nullptr, 0.0f);
        g_materialPostProcessing.setMaps(nullptr, nullptr, nullptr, 0.0f);

        g_materialContainer.setMaps(nullptr, nullptr, nullptr, 0.0f);
        g_materialMetalRimmedContainer.setMaps(nullptr, nullptr, nullptr, 0.0f);
        g_materialSteel.setMaps(nullptr, nullptr, nullptr, 0.0f);
        g_materialBrickWall.setMaps(nullptr, nullptr, nullptr, 0.0f);

        g_materialsInit = false;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownMaterials: Materials not initialized. Aborting operation, nothing was modified");
    }
}

void setupMeshes() {
    if(!g_meshesInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownMeshes: Creating meshes");

        if(!g_windowInit) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "setupMeshes: Window not initialized. Aborting operation, nothing was modified");
            return;
        }

        static float cubeData[] = {
    //  Position             Tex Coods    Tangent            Bitangent          Normal
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  1.0f,

       -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f
        };
        static int cubeDataSize = sizeof(cubeData);

        static unsigned int indices[] = {
            0, 2, 1,
            3, 5, 4,

            6, 7, 8,
            9, 10, 11,

            12, 13, 14,
            15, 16, 17,

            18, 20, 19,
            21, 23, 22,

            24, 25, 26,
            27, 28, 29,

            30, 32, 31,
            33, 35, 34
        };
        static int indicesSize = sizeof(indices);

        static float quadData[] = {
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };
        static int quadDataSize = sizeof(quadData);

        static unsigned int indicesQuad[] = {
            0, 1, 2,
            2, 3, 0
        };
        static int indicesQuadSize = sizeof(indicesQuad);

        g_meshCube = prMeshCreate();
        prMeshSetVertexAttribute(g_meshCube, 0, 3, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
        prMeshSetVertexAttribute(g_meshCube, 1, 2, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        prMeshSetVertexAttribute(g_meshCube, 2, 3, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
        prMeshSetVertexAttribute(g_meshCube, 3, 3, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
        prMeshSetVertexAttribute(g_meshCube, 4, 3, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));
        prMeshUpdate(g_meshCube, cubeData, cubeDataSize, indices, indicesSize);
        prMeshLinkContext(g_meshCube, g_window->openglContext);

        g_meshQuad = prMeshCreate();
        prMeshSetVertexAttribute(g_meshQuad, 0, 2, PR_FLOAT, PR_FALSE, 4 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
        prMeshSetVertexAttribute(g_meshQuad, 1, 2, PR_FLOAT, PR_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
        prMeshUpdate(g_meshQuad, quadData, quadDataSize, indicesQuad, indicesQuadSize);
        prMeshLinkContext(g_meshQuad, g_window->openglContext);

        g_meshesInit = true;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownMeshes: Meshes already initialized. Aborting operation, nothing was modified");
    }
}

void shutdownMeshes() {
    if(g_meshesInit) {
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "shutdownMeshes: Destroying meshes");

        prMeshDestroy(g_meshCube);
        g_meshCube = nullptr;
        prMeshDestroy(g_meshQuad);
        g_meshQuad = nullptr;

        g_meshesInit = false;
    } else {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "shutdownMeshes: Meshes not initialized. Aborting operation, nothing was modified");
    }
}
