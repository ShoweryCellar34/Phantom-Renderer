#include <PR/PR.h>
#include <PR/helperStuffs.h>

#include <stdio.h>
#include <cglm/cglm.h>
#include <stb_image.h>
#include <PR/memory.h>
#include "exampleGlobalValues.h"
#include "exampleFunctions.h"

int main(int argc, char** argv) {
    prLogSetLevel(PR_LOG_TRACE);
    FILE* logFile = fopen("prLog.txt", "w");
    prLogSetStream(2, (FILE*[]){stdout, logFile});

    glfwInit();

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    prWindow* test = prWindowCreate(TITLE, windowWidth, windowHeight);
    prWindowInitContext(test);

    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);
    glfwSetCursorPosCallback(test->window, cursorPosCallback);
    glfwSetKeyCallback(test->window, keyCallback);
    glfwSetInputMode(test->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    test->openglContext->Enable(GL_DEBUG_OUTPUT);
    test->openglContext->Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    test->openglContext->DebugMessageCallback(openglDebugOutput, NULL);
    test->openglContext->DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    glfwSetWindowUserPointer(test->window, test->openglContext);

    stbi_set_flip_vertically_on_load(1);

    prShaderData* shaderProgram = loadShader(test->openglContext, "res/shaders/defaultVertexShader.glsl", "res/shaders/defaultFragmentShader.glsl", "res/shaders/defaultGeometryShader.glsl");
    debugShaderProgram = loadShader(test->openglContext, "res/shaders/debugVertexShader.glsl", "res/shaders/debugFragmentShader.glsl", "res/shaders/debugGeometryShader.glsl");

    prShaderData* depthShaderProgram = loadShader(test->openglContext, "res/shaders/depthVertexShader.glsl", "res/shaders/depthFragmentShader.glsl", NULL);
    prShaderData* depth2ShaderProgram = loadShader(test->openglContext, "res/shaders/depth2VertexShader.glsl", "res/shaders/depth2FragmentShader.glsl", "res/shaders/depth2GeometryShader.glsl");

    prShaderData* skyboxShaderProgram = loadShader(test->openglContext, "res/shaders/skyboxVertexShader.glsl", "res/shaders/skyboxFragmentShader.glsl", NULL);

    prShaderData* hudShaderProgram = loadShader(test->openglContext, "res/shaders/HUDVertexShader.glsl", "res/shaders/HUDFragmentShader.glsl", NULL);

    prShaderData* gaussianShaderProgram = loadShader(test->openglContext, "res/shaders/HUDVertexShader.glsl", "res/shaders/gaussianFragmentShader.glsl", NULL);

    prShaderData* hdrShaderProgram = loadShader(test->openglContext, "res/shaders/hdrVertexShader.glsl", "res/shaders/hdrFragmentShader.glsl", NULL);

    computeShaderProgram = loadComputeShader(test->openglContext, "res/shaders/postProcessingComputeShader.glsl");

    prTextureData* defaultTexture = makeTextureCheckerboard(test->openglContext, 8, (float[4]){1.0f, 0.0f, 1.0f, 1.0f}, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});

    prTextureData* containerTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, "res/textures/container.jpg");

    prTextureData* containerMetalTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, "res/textures/container2.png");

    prTextureData* containerMetalSpecularTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, false, "res/textures/container2_specular.png");

    prTextureData* steelTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, "res/textures/steel.jpg");

    prTextureData* steelNormal = loadTexture(test->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, false, "res/textures/steelNormal.png");

    prTextureData* brickWallDiffuseTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, "res/textures/brickwall.jpg");

    prTextureData* brickWallNormalTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, false, "res/textures/brickwall_normal.jpg");

    prTextureData* blackTexture = makeTextureSingleColor(test->openglContext, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});

    prTextureData* whiteTexture = makeTextureSingleColor(test->openglContext, (float[4]){1.0f, 1.0f, 1.0f, 1.0f});

    prTextureData* defaultNormal = makeTextureSingleColor(test->openglContext, (float[4]){0.0f, -1.0f, 0.0f, 1.0f});

    prTextureData* HUDTexture = loadTexture(test->openglContext, PR_FILTER_NEAREST_MIPMAP_NEAREST, PR_FILTER_LINEAR, true, "res/textures/HUD.png");

    prTextureData* depthTextureDepth = prTextureCreate();
    prTextureLinkContext(depthTextureDepth, test->openglContext);
    prTextureUpdate(depthTextureDepth, PR_FORMAT_DEPTH, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, PR_FILTER_LINEAR, NULL, 0, 2048, 2048);
    prTextureBorderColor(depthTextureDepth, (GLfloat[]){1.0f, 0.0f, 0.0f, 0.0f});

    prFramebufferData* framebufferDepth = prFramebufferCreate();
    prFramebufferLinkContext(framebufferDepth, test->openglContext);
    prFramebufferLinkDepthTexture(framebufferDepth, depthTextureDepth);
    prFramebufferSetDrawBuffer(framebufferDepth, PR_NONE);
    prFramebufferSetReadBuffer(framebufferDepth, PR_NONE);

    prCubeMapData* depthCubeMapDepth2 = prCubeMapCreate();
    prCubeMapLinkContext(depthCubeMapDepth2, test->openglContext);
    prCubeMapUpdateAll(depthCubeMapDepth2,
        (GLenum[]){PR_FORMAT_DEPTH, PR_FORMAT_DEPTH, PR_FORMAT_DEPTH, PR_FORMAT_DEPTH, PR_FORMAT_DEPTH, PR_FORMAT_DEPTH},
        PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_LINEAR, PR_FILTER_LINEAR,
        (GLubyte*[]){NULL, NULL, NULL, NULL, NULL, NULL},
        (size_t[]){0, 0, 0, 0, 0, 0},
        (GLsizei[]){1024, 1024, 1024, 1024, 1024, 1024},
        (GLsizei[]){1024, 1024, 1024, 1024, 1024, 1024});
    prCubeMapBorderColor(depthCubeMapDepth2, (GLfloat[]){1.0f, 0.0f, 0.0f, 0.0f});

    prFramebufferData* framebufferDepth2 = prFramebufferCreate();
    prFramebufferLinkContext(framebufferDepth2, test->openglContext);
    prFramebufferLinkDepthCubeMap(framebufferDepth2, depthCubeMapDepth2);
    prFramebufferSetDrawBuffer(framebufferDepth2, GL_NONE);
    prFramebufferSetReadBuffer(framebufferDepth2, GL_NONE);

    test->openglContext->Enable(GL_MULTISAMPLE);
    colorRBOMultisampled = prRenderBufferCreate();
    prRenderBufferLinkContext(colorRBOMultisampled, test->openglContext);
    prRenderBufferUpdate(colorRBOMultisampled, PR_FORMAT_RGBA, windowWidth, windowHeight, SAMPLES);

    colorMultisamlpedTexture2 = prTextureMultisampledCreate();
    prTextureMultisampledLinkContext(colorMultisamlpedTexture2, test->openglContext);
    prTextureMultisampledUpdate(colorMultisamlpedTexture2, PR_FORMAT_RGBA, windowWidth, windowHeight, SAMPLES);

    depthStencilRBOMultisampled = prRenderBufferCreate();
    prRenderBufferLinkContext(depthStencilRBOMultisampled, test->openglContext);
    prRenderBufferUpdate(depthStencilRBOMultisampled, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight, SAMPLES);

    colorTexture2 = prTextureCreate();
    prTextureLinkContext(colorTexture2, test->openglContext);
    prTextureUpdate(colorTexture2, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);

    prFramebufferData* framebuffer2 = prFramebufferCreate();
    prFramebufferLinkContext(framebuffer2, test->openglContext);
    prFramebufferLinkColorTexture(framebuffer2, colorTexture2, 0);

    colorTexture3 = prTextureCreate();
    prTextureLinkContext(colorTexture3, test->openglContext);
    prTextureUpdate(colorTexture3, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);

    prFramebufferData* framebuffer3 = prFramebufferCreate();
    prFramebufferLinkContext(framebuffer3, test->openglContext);
    prFramebufferLinkColorTexture(framebuffer3, colorTexture3, 0);

    prFramebufferData* gaussianFramebuffers[] = {framebuffer2, framebuffer3};
    prTextureData* gaussianTextures[] = {colorTexture2, colorTexture3};

    framebufferMultisampled = prFramebufferCreate();
    prFramebufferLinkContext(framebufferMultisampled, test->openglContext);
    prFramebufferLinkColorRBO(framebufferMultisampled, colorRBOMultisampled, 0);
    prFramebufferLinkColorTextureMultisampled(framebufferMultisampled, colorMultisamlpedTexture2, 1);
    prFramebufferDrawBuffers(framebufferMultisampled, 2, (GLenum[]){PR_COLOR_ATTACHMENT_0, PR_COLOR_ATTACHMENT_1});
    prFramebufferLinkDepthStencilRBO(framebufferMultisampled, depthStencilRBOMultisampled);

    bloomTexture = prTextureCreate();
    prTextureLinkContext(bloomTexture, test->openglContext);
    prTextureUpdate(bloomTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);

    prFramebufferData* bloomFramebuffer = prFramebufferCreate();
    prFramebufferLinkContext(bloomFramebuffer, test->openglContext);
    prFramebufferLinkColorTexture(bloomFramebuffer, bloomTexture, 0);

    postProcessingTexture = prTextureCreate();
    prTextureLinkContext(postProcessingTexture, test->openglContext);
    prTextureUpdate(postProcessingTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);

    colorTexture = prTextureCreate();
    prTextureLinkContext(colorTexture, test->openglContext);
    prTextureUpdate(colorTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR_MIPMAP_NEAREST, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);

    depthStencilRBO = prRenderBufferCreate();
    prRenderBufferLinkContext(depthStencilRBO, test->openglContext);
    prRenderBufferUpdate(depthStencilRBO, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight, 0);

    framebuffer = prFramebufferCreate();
    prFramebufferLinkContext(framebuffer, test->openglContext);
    prFramebufferLinkColorTexture(framebuffer, colorTexture, 0);
    prFramebufferLinkDepthStencilRBO(framebuffer, depthStencilRBO);

    prCubeMapData* skyboxDefaultCubeMap = makeCubeMapSingleColors(test->openglContext, (float[PR_CUBE_MAP_SIDES][4]){
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    });

    stbi_set_flip_vertically_on_load(0);
    prCubeMapData* skyboxCubeMap1 = loadCubeMap(test->openglContext, PR_FILTER_LINEAR_MIPMAP_LINEAR, PR_FILTER_LINEAR, skybox1Textures);
    prCubeMapData* skyboxCubeMap2 = loadCubeMap(test->openglContext, PR_FILTER_LINEAR_MIPMAP_LINEAR, PR_FILTER_LINEAR, skybox2Textures);
    stbi_set_flip_vertically_on_load(1);

    prCubeMapData* skyboxCubeMap3 = makeCubeMapSingleColors(test->openglContext, (float[PR_CUBE_MAP_SIDES][4]){
        {1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 1.0f, 1.0f}
    });

    prCubeMapData* skybox4CubeMap = makeCubeMapCheckerboards(test->openglContext, 32, (float[PR_CUBE_MAP_SIDES][4]){
        {1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 1.0f, 1.0f}
    }, (float[PR_CUBE_MAP_SIDES][4]){
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    });

    materialData defaultMaterial = {
        defaultTexture,
        defaultTexture,
        defaultTexture,
        defaultNormal,
        48.0f
    };

    materialData materialWhite = {
        whiteTexture,
        whiteTexture,
        whiteTexture,
        defaultNormal,
        0.0f,
    };

    materialData materialMetal = {
        steelTexture,
        steelTexture,
        whiteTexture,
        steelNormal,
        256.0f
    };

    materialData materialWood = {
        containerTexture,
        containerTexture,
        whiteTexture,
        defaultNormal,
        16.0f
    };

    materialData materialWoodMetal = {
        containerMetalTexture,
        containerMetalTexture,
        containerMetalSpecularTexture,
        defaultNormal,
        256.0f
    };

    materialData materialBrick = {
        brickWallDiffuseTexture,
        brickWallDiffuseTexture,
        whiteTexture,
        brickWallNormalTexture,
        32.0f
    };

    materialData materialHUD = {
        HUDTexture,
        blackTexture,
        blackTexture,
        blackTexture,
        0.0f
    };

    materialData materialPostProcessing = {
        postProcessingTexture,
        blackTexture,
        blackTexture,
        blackTexture,
        0.0f
    };

    prMeshData* meshCube = prMeshCreate();
    prMeshLinkContext(meshCube, test->openglContext);
    prMeshSetVertexAttribute(meshCube, 0, 3, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    prMeshSetVertexAttribute(meshCube, 1, 2, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    prMeshSetVertexAttribute(meshCube, 2, 3, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    prMeshSetVertexAttribute(meshCube, 3, 3, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
    prMeshSetVertexAttribute(meshCube, 4, 3, PR_FLOAT, PR_FALSE, 14 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));
    prMeshUpdate(meshCube,
        cubeData, cubeDataSize,
        indices, indicesSize);

    prMeshData* meshQuad = prMeshCreate();
    prMeshLinkContext(meshQuad, test->openglContext);
    prMeshSetVertexAttribute(meshQuad, 0, 2, PR_FLOAT, PR_FALSE, 4 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
    prMeshSetVertexAttribute(meshQuad, 1, 2, PR_FLOAT, PR_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    prMeshUpdate(meshQuad,
        quadData, quadDataSize,
        indicesQuad, indicesQuadSize);

    typedef struct directionalLightData {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        GLuint shadowMap;
    } directionalLightData;

    directionalLightData sun = {
        {-0.25f, -0.5f, -0.75f},
        {0.02f, 0.015f, 0.015f},
        {0.6f, 0.6f, 0.55f},
        {1.3f, 1.3f, 1.25f},
        4
    };

    typedef struct pointLightData {
        float constant;
        float linear;
        float quadratic;

        vec3 position;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        GLuint shadowMap;
        float farPlane;
    } pointLightData;

    pointLightData point = {
        1.0f,
        0.22f,
        0.2f,
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.5f, 0.0f, 0.0f},
        5,
        100.0f
    };

    camera = prCameraCreate();
    prCameraLinkContext(camera, test->openglContext);

    mat4 lightProjection;
    glm_ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f, lightProjection);
    mat4 lightView;
    glm_lookat((vec3){40.0f, 40.0f, 40.0f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, lightView);
    mat4 lightSpaceMatrix;
    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);
    prShaderSetUniformMatrix4fv(depthShaderProgram, "lightSpaceMatrix", lightSpaceMatrix[0]);

    float aspect = (float)1024 / (float)1024;
    mat4 light2Projection;
    glm_perspective(glm_rad(90.0f), aspect, 0.1f, 100.0f, light2Projection);
    mat4 light2View[6];
    glm_lookat(point.position, (vec3){point.position[0] + 1.0f, point.position[1], point.position[2]}, (vec3){0.0f, -1.0f, 0.0f}, light2View[0]);
    glm_lookat(point.position, (vec3){point.position[0] + -1.0f, point.position[1], point.position[2]}, (vec3){0.0f, -1.0f, 0.0f}, light2View[1]);
    glm_lookat(point.position, (vec3){point.position[0], point.position[1] + 1.0f, point.position[2]}, (vec3){0.0f, 0.0f, 1.0f}, light2View[2]);
    glm_lookat(point.position, (vec3){point.position[0], point.position[1] + -1.0f, point.position[2]}, (vec3){0.0f, 0.0f, -1.0f}, light2View[3]);
    glm_lookat(point.position, (vec3){point.position[0], point.position[1], point.position[2] + 1.0f}, (vec3){0.0f, -1.0f, 0.0f}, light2View[4]);
    glm_lookat(point.position, (vec3){point.position[0], point.position[1], point.position[2] + -1.0f}, (vec3){0.0f, -1.0f, 0.0f}, light2View[5]);
    mat4 light2SpaceMatrix[6];
    glm_mat4_mul(light2Projection, light2View[0], light2SpaceMatrix[0]);
    glm_mat4_mul(light2Projection, light2View[1], light2SpaceMatrix[1]);
    glm_mat4_mul(light2Projection, light2View[2], light2SpaceMatrix[2]);
    glm_mat4_mul(light2Projection, light2View[3], light2SpaceMatrix[3]);
    glm_mat4_mul(light2Projection, light2View[4], light2SpaceMatrix[4]);
    glm_mat4_mul(light2Projection, light2View[5], light2SpaceMatrix[5]);
    prShaderSetUniformMatrix4fv(depth2ShaderProgram, "lightSpaceMatrices[0]", light2SpaceMatrix[0][0]);
    prShaderSetUniformMatrix4fv(depth2ShaderProgram, "lightSpaceMatrices[1]", light2SpaceMatrix[1][0]);
    prShaderSetUniformMatrix4fv(depth2ShaderProgram, "lightSpaceMatrices[2]", light2SpaceMatrix[2][0]);
    prShaderSetUniformMatrix4fv(depth2ShaderProgram, "lightSpaceMatrices[3]", light2SpaceMatrix[3][0]);
    prShaderSetUniformMatrix4fv(depth2ShaderProgram, "lightSpaceMatrices[4]", light2SpaceMatrix[4][0]);
    prShaderSetUniformMatrix4fv(depth2ShaderProgram, "lightSpaceMatrices[5]", light2SpaceMatrix[5][0]);
    prShaderSetUniform3f(depth2ShaderProgram, "lightPosition", point.position[0], point.position[1], point.position[2]);
    prShaderSetUniform1f(depth2ShaderProgram, "farPlane", 100.0f);

    test->openglContext->Enable(GL_DEPTH_TEST);
    test->openglContext->Enable(GL_BLEND);

    glfwMaximizeWindow(test->window);

    prShaderSetUniform2f(debugShaderProgram, "screenSize", windowWidth, windowHeight);

    while(!glfwWindowShouldClose(test->window)) {
        test->openglContext->Enable(GL_DEPTH_TEST);
        prFramebufferClearColor(test->openglContext, NULL, 0, (GLfloat[]){0.3f, 0.5f, 0.7f, 1.0f});
        prFramebufferClearDepth(test->openglContext, NULL, 1.0f);
        prFramebufferClearDepth(test->openglContext, framebufferDepth, 1.0f);
        prFramebufferClearDepth(test->openglContext, framebufferDepth2, 1.0f);
        prFramebufferClearColor(test->openglContext, framebuffer2, 0, (GLfloat[]){0.0f, 0.0f, 0.0f, 0.0f});
        prFramebufferClearColor(test->openglContext, framebuffer3, 0, (GLfloat[]){0.0f, 0.0f, 0.0f, 0.0f});
        prFramebufferClearColor(test->openglContext, bloomFramebuffer, 0, (GLfloat[]){0.0f, 0.0f, 0.0f, 0.0f});
        prFramebufferClearColor(test->openglContext, framebufferMultisampled, 0, (GLfloat[]){0.7f, 0.5f, 0.3f, 1.0f});
        prFramebufferClearColor(test->openglContext, framebufferMultisampled, 1, (GLfloat[]){0.7f, 0.5f, 0.3f, 1.0f});
        prFramebufferClearDepthStencil(test->openglContext, framebufferMultisampled, 1.0f, 0);
        prFramebufferClearColor(test->openglContext, framebuffer, 0, (GLfloat[]){0.3f, 0.5f, 0.7f, 1.0f});
        prFramebufferClearDepthStencil(test->openglContext, framebuffer, 1.0f, 0);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        mat4 translation;

        vec3 rotation = {glm_rad(yaw), glm_rad(pitch), glm_rad(0.0f)};
        prCameraUpdate(camera, cameraPosition, rotation, 45.0f, 0.1f, 1500.0f);

        prShaderData* currentShaderProgram = (useDebugShader ? debugShaderProgram : shaderProgram);

        static float seed = 0;
        seed += deltaTime;
        float smoothSinOverTime = sin(seed);

        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].direction", sun.direction[0], sun.direction[1], sun.direction[2]);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].ambient", sun.ambient[0], sun.ambient[1], sun.ambient[2]);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].diffuse", sun.diffuse[0], sun.diffuse[1], sun.diffuse[2]);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].specular", sun.specular[0], sun.specular[1], sun.specular[2]);
        prTextureBindTexture(depthTextureDepth, sun.shadowMap);
        prShaderSetUniform1i(currentShaderProgram, "directionalLights[0].shadowMap", sun.shadowMap);

        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].constant", point.constant);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].linear", point.linear);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].quadratic", point.quadratic);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].position", point.position[0], point.position[1], point.position[2]);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].ambient", point.ambient[0], point.ambient[1], point.ambient[2]);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].diffuse", point.diffuse[0], point.diffuse[1], point.diffuse[2]);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].specular", point.specular[0], point.specular[1], point.specular[2]);
        prCubeMapBindTexture(depthCubeMapDepth2, point.shadowMap);
        prShaderSetUniform1i(currentShaderProgram, "pointLights[0].shadowMap", point.shadowMap);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].farPlane", point.farPlane);

        prShaderSetUniform3f(currentShaderProgram, "cameraPosition", camera->position[0], camera->position[1], camera->position[2]);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "view", camera->view[0]);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "projection", camera->projection[0]);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "lightSpaceMatrix", lightSpaceMatrix[0]);

        for(int i = 0; i < 3; i++) {
            switch(i) {
                case 0:
                    prFramebufferBind(framebufferDepth);
                    currentShaderProgram = depthShaderProgram;
                    test->openglContext->Viewport(0, 0, 2048, 2048);
                    break;

                case 1:
                    prFramebufferBind(framebufferDepth2);
                    currentShaderProgram = depth2ShaderProgram;
                    test->openglContext->Viewport(0, 0, 1024, 1024);
                    break;

                case 2:
                    prFramebufferBind(framebufferMultisampled);
                    currentShaderProgram = (useDebugShader ? debugShaderProgram : shaderProgram);
                    test->openglContext->Viewport(0, 0, windowWidth, windowHeight);
                    break;
            }

            translationsToMatrix(translation, (vec3){0.0f, 0.0f, -30.0f}, GLM_VEC3_ZERO, (vec3){30.0f, 30.0f, 30.0f});
            if(i == 2) {
                bindMaterial(&materialWood, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);

            translationsToMatrix(translation, (vec3){0.0f, -30.0f, 0.0f}, GLM_VEC3_ZERO, (vec3){30.0f, 30.0f, 30.0f});
            if(i == 2) {
                bindMaterial(&materialMetal, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);

            translationsToMatrix(translation, (vec3){-30.0f, 0.0f, 0.0f}, GLM_VEC3_ZERO, (vec3){30.0f, 30.0f, 30.0f});
            if(i == 2) {
                bindMaterial(&materialBrick, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);

            translationsToMatrix(translation, (vec3){2.0f, 0.0f, 0.0f}, (vec3){0.0f, smoothSinOverTime, 0.0f}, GLM_VEC3_ONE);
            if(i == 2) {
                bindMaterial(&materialWood, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);

            translationsToMatrix(translation, (vec3){-2.0f, 0.0f, 0.0f}, (vec3){0.0f, smoothSinOverTime, 0.0f}, GLM_VEC3_ONE);
            if(i == 2) {
                bindMaterial(&materialWoodMetal, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);

            translationsToMatrix(translation, (vec3){0.0f, 2.0f, 0.0f}, (vec3){0.0f, smoothSinOverTime, 0.0f}, GLM_VEC3_ONE);
            if(i == 2) {
                bindMaterial(&materialBrick, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);

            translationsToMatrix(translation, (vec3){0.0f, -2.0f, 0.0f}, (vec3){0.0f, glm_rad(smoothSinOverTime * 100.0f), 0.0f}, GLM_VEC3_ONE);
            if(i == 2) {
                bindMaterial(&defaultMaterial, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);

            translationsToMatrix(translation, (vec3){0.0f, 0.0f, 2.0f}, (vec3){0.0f, glm_rad(smoothSinOverTime * 100.0f), 0.0f}, GLM_VEC3_ONE);
            if(i == 2) {
                bindMaterial(&materialMetal, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);

            translationsToMatrix(translation, (vec3){0.0f, 0.0f, -2.0f}, (vec3){0.0f, glm_rad(smoothSinOverTime * 100.0f), 0.0f}, GLM_VEC3_ONE);
            if(i == 2) {
                bindMaterial(&materialMetal, currentShaderProgram);
            }
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(meshCube);
        }

        switch(currentSkybox) {
            case 1:
                prCubeMapBindTexture(skyboxCubeMap1, 0);
                break;

            case 2:
                prCubeMapBindTexture(skyboxCubeMap2, 0);
                break;

            case 3:
                prCubeMapBindTexture(skyboxCubeMap3, 0);
                break;

            case 4:
                prCubeMapBindTexture(skybox4CubeMap, 0);
                break;

            default:
                prCubeMapBindTexture(skyboxDefaultCubeMap, 0);
                break;
        }

        translationsToMatrix(translation, camera->position, GLM_VEC3_ZERO, (vec3){1.0f, 1.0f, 1.0f});
        prShaderSetUniformMatrix4fv(skyboxShaderProgram, "translation", translation[0]);
        prShaderSetUniformMatrix4fv(skyboxShaderProgram, "view", camera->view[0]);
        prShaderSetUniformMatrix4fv(skyboxShaderProgram, "projection", camera->projection[0]);
        test->openglContext->DepthFunc(GL_LEQUAL);
        prShaderBind(skyboxShaderProgram);
        prMeshDrawIndices(meshCube);

        if(showHUD == 1) {
            test->openglContext->Disable(GL_DEPTH_TEST);
            bindMaterialAmbientOnly(&materialHUD, hudShaderProgram);
            prMeshDrawIndices(meshQuad);
        }

        prFramebufferBlit(test->openglContext, framebufferMultisampled, framebuffer,
            0, 0, windowWidth, windowHeight,
            0, 0, windowWidth, windowHeight,
            PR_BUFFER_BIT_COLOR, PR_FILTER_NEAREST
        );
        prFramebufferBind(framebuffer);

        if(showPostProcessing) {
            test->openglContext->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            prTextureBindTexture(colorTexture, 0);
            prTextureBindImage(postProcessingTexture, 1, 0, PR_ACCESS_WRITE_ONLY, GL_RGBA32F);
            prComputeShaderDispatch(computeShaderProgram, windowWidth , windowHeight, 1);
            test->openglContext->MemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

            bindMaterialAmbientOnly(&materialPostProcessing, hudShaderProgram);
            prMeshDrawIndices(meshQuad);
        }

        prFramebufferSetReadBuffer(framebufferMultisampled, PR_COLOR_ATTACHMENT_1);
        prFramebufferBlit(test->openglContext, framebufferMultisampled, bloomFramebuffer,
            0, 0, windowWidth, windowHeight,
            0, 0, windowWidth, windowHeight,
            PR_BUFFER_BIT_COLOR, PR_FILTER_NEAREST
        );
        prFramebufferSetReadBuffer(framebufferMultisampled, PR_COLOR_ATTACHMENT_0);
        bool horizontal = true, firstIteration = true;
        int amount = BLOOM_BLUR_PASSES;
        prShaderBind(gaussianShaderProgram);
        prShaderSetUniform1i(gaussianShaderProgram, "image", 0);
        for(int i = 0; i < amount; i++) {
            prFramebufferBind(gaussianFramebuffers[horizontal]);
            prShaderSetUniform1i(gaussianShaderProgram, "horizontal", horizontal);
            prTextureBindTexture(firstIteration == true ? bloomTexture : gaussianTextures[!horizontal], 0);
            test->openglContext->Disable(GL_DEPTH_TEST);
            prMeshDrawIndices(meshQuad);
            horizontal = !horizontal;
            if(firstIteration) {
                firstIteration = false;
            }
        }
        prFramebufferBind(framebuffer);

        test->openglContext->Disable(GL_DEPTH_TEST);
        prTextureBindTexture(colorTexture, 0);
        prTextureBindTexture(gaussianTextures[!horizontal], 1);
        prShaderSetUniform1i(hdrShaderProgram, "scene", 0);
        prShaderSetUniform1i(hdrShaderProgram, "bloomBlur", 1);
        prShaderSetUniform1f(hdrShaderProgram, "exposure", 1.5);
        prShaderBind(hdrShaderProgram);
        prMeshDrawIndices(meshQuad);

        prFramebufferBlit(test->openglContext, framebuffer, NULL,
            0, 0, windowWidth, windowHeight,
            0, 0, windowWidth, windowHeight,
            PR_BUFFER_BIT_COLOR, PR_FILTER_NEAREST
        );

        glfwSwapBuffers(test->window);
        glfwPollEvents();
        proccessInput(test->window);
    }

    prCameraDestroy(camera);
    camera = NULL;

    prMeshDestroy(meshQuad);
    meshQuad = NULL;
    prMeshDestroy(meshCube);
    meshCube = NULL;

    prFramebufferDestroy(framebuffer);
    framebuffer = NULL;
    prRenderBufferDestroy(depthStencilRBO);
    depthStencilRBO = NULL;
    prTextureDestroy(colorTexture);
    colorTexture = NULL;
    prFramebufferDestroy(framebufferMultisampled);
    framebufferMultisampled = NULL;
    prFramebufferDestroy(bloomFramebuffer);
    bloomFramebuffer = NULL;
    prTextureDestroy(bloomTexture);
    bloomTexture = NULL;
    prFramebufferDestroy(framebuffer3);
    framebuffer3 = NULL;
    prTextureDestroy(colorTexture3);
    colorTexture3 = NULL;
    prFramebufferDestroy(framebuffer2);
    framebuffer2 = NULL;
    prTextureDestroy(colorTexture2);
    colorTexture2 = NULL;
    prRenderBufferDestroy(depthStencilRBOMultisampled);
    depthStencilRBOMultisampled = NULL;
    prTextureMultisampledDestroy(colorMultisamlpedTexture2);
    colorMultisamlpedTexture2 = NULL;
    prRenderBufferDestroy(colorRBOMultisampled);
    colorRBOMultisampled = NULL;
    prFramebufferDestroy(framebufferDepth2);
    framebufferDepth2 = NULL;
    prCubeMapDestroy(depthCubeMapDepth2);
    depthCubeMapDepth2 = NULL;
    prFramebufferDestroy(framebufferDepth);
    framebufferDepth = NULL;
    prTextureDestroy(depthTextureDepth);
    depthTextureDepth = NULL;

    prCubeMapDestroy(skybox4CubeMap);
    skybox4CubeMap = NULL;
    prCubeMapDestroy(skyboxCubeMap3);
    skyboxCubeMap3 = NULL;
    prCubeMapDestroy(skyboxCubeMap2);
    skyboxCubeMap2 = NULL;
    prCubeMapDestroy(skyboxCubeMap1);
    skyboxCubeMap1 = NULL;
    prCubeMapDestroy(skyboxDefaultCubeMap);
    skyboxDefaultCubeMap = NULL;
    prTextureDestroy(postProcessingTexture);
    postProcessingTexture = NULL;
    prTextureDestroy(HUDTexture);
    HUDTexture = NULL;
    prTextureDestroy(defaultNormal);
    defaultNormal = NULL;
    prTextureDestroy(whiteTexture);
    whiteTexture = NULL;
    prTextureDestroy(blackTexture);
    blackTexture = NULL;
    prTextureDestroy(brickWallNormalTexture);
    brickWallNormalTexture = NULL;
    prTextureDestroy(brickWallDiffuseTexture);
    brickWallDiffuseTexture = NULL;
    prTextureDestroy(steelTexture);
    steelTexture = NULL;
    prTextureDestroy(containerMetalSpecularTexture);
    containerMetalSpecularTexture = NULL;
    prTextureDestroy(containerMetalTexture);
    containerMetalTexture = NULL;
    prTextureDestroy(containerTexture);
    containerTexture = NULL;
    prTextureDestroy(defaultTexture);
    defaultTexture = NULL;

    prComputeShaderDestroy(computeShaderProgram);
    computeShaderProgram = NULL;
    prShaderDestroy(hdrShaderProgram);
    hdrShaderProgram = NULL;
    prShaderDestroy(gaussianShaderProgram);
    gaussianShaderProgram = NULL;
    prShaderDestroy(hudShaderProgram);
    hudShaderProgram = NULL;
    prShaderDestroy(skyboxShaderProgram);
    skyboxShaderProgram = NULL;
    prShaderDestroy(depth2ShaderProgram);
    depth2ShaderProgram = NULL;
    prShaderDestroy(depthShaderProgram);
    depthShaderProgram = NULL;
    prShaderDestroy(debugShaderProgram);
    debugShaderProgram = NULL;
    prShaderDestroy(shaderProgram);
    shaderProgram = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    fclose(logFile);
    logFile = NULL;
}
