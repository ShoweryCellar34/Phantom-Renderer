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
    test->openglContext->DebugMessageCallback(glDebugOutput, NULL);
    test->openglContext->DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    glfwSetWindowUserPointer(test->window, test->openglContext);

    stbi_set_flip_vertically_on_load(1);

    prShaderData* shaderProgram = createDefaultShader(test->openglContext);

    prShaderData* skyboxShaderProgram = prShaderCreate();
    prShaderLinkContext(skyboxShaderProgram, test->openglContext);
    prShaderUpdate(skyboxShaderProgram, SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER);

    prShaderData* hudShaderProgram = prShaderCreate();
    prShaderLinkContext(hudShaderProgram, test->openglContext);
    prShaderUpdate(hudShaderProgram, HUD_VERTEX_SHADER, HUD_FRAGMENT_SHADER);

    computeShaderProgram = prComputeShaderCreate();
    prComputeShaderLinkContext(computeShaderProgram, test->openglContext);
    prComputeShaderUpdate(computeShaderProgram, HUD_COMPUTE_SHADER);

    prTextureData* defaultTexture = makeTextureCheckerboard(test->openglContext, 8, (float[4]){1.0f, 0.0f, 1.0f, 1.0f}, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});

    prTextureData* containerTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR, "res/container.jpg");

    prTextureData* containerMetalTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR, "res/container2.png");

    prTextureData* containerMetalSpecularTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR, "res/container2_specular.png");

    prTextureData* steelTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR, "res/steel.jpg");

    prTextureData* steelNormal = loadTexture(test->openglContext, PR_FILTER_LINEAR, "res/steelNormal.png");

    prTextureData* brickWallDiffuseTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR, "res/brickWallDiffuse.tga");

    prTextureData* brickWallNormalTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR, "res/brickWallNormal.tga");

    prTextureData* brickWallSpecularTexture = loadTexture(test->openglContext, PR_FILTER_LINEAR, "res/brickWallSpecular.png");

    prTextureData* blackTexture = makeTextureSingleColor(test->openglContext, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});

    prTextureData* whiteTexture = makeTextureSingleColor(test->openglContext, (float[4]){1.0f, 1.0f, 1.0f, 1.0f});

    prTextureData* defaultNormal = makeTextureSingleColor(test->openglContext, (float[4]){0.0f, -1.0f, 0.0f, 1.0f});

    prTextureData* HUDTexture = loadTexture(test->openglContext, PR_FILTER_NEAREST, "res/HUD.png");

    postProcessingTexture = prTextureCreate();
    prTextureLinkContext(postProcessingTexture, test->openglContext);
    prTextureUpdate(postProcessingTexture, PR_FORMAT_RGBA, PR_FILTER_LINEAR, PR_WRAPPING_EDGE, NULL, 0, windowWidth, windowHeight);

    colorTexture = prTextureCreate();
    prTextureLinkContext(colorTexture, test->openglContext);
    prTextureUpdate(colorTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);

    depthStencilRBO = prRenderBufferCreate();
    prRenderBufferLinkContext(depthStencilRBO, test->openglContext);
    prRenderBufferUpdate(depthStencilRBO, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight);

    framebuffer = prFramebufferCreate();
    prFramebufferLinkContext(framebuffer, test->openglContext);
    prFramebufferLinkColorTexture(framebuffer, colorTexture);
    prFramebufferLinkDepthStencilTextureRBO(framebuffer, depthStencilRBO);

    prCubeMapData* skyboxDefaultCubeMap = makeCubeMapSingleColors(test->openglContext, (float[PR_CUBE_MAP_SIDES][4]){
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    });

    stbi_set_flip_vertically_on_load(0);
    prCubeMapData* skyboxCubeMap1 = loadCubeMap(test->openglContext, PR_FILTER_LINEAR, skybox1Textures);
    prCubeMapData* skyboxCubeMap2 = loadCubeMap(test->openglContext, PR_FILTER_LINEAR, skybox2Textures);
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

    skyboxMaterialData materialSkybox = {
        defaultTexture,
        defaultTexture,
        defaultTexture,
        defaultTexture,
        defaultTexture,
        defaultTexture
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
        brickWallSpecularTexture,
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

    cubeData = computeGPUReadyBuffer(&cubeDataSize,
        vertices, sizeof(vertices) / sizeof(float),
        normals, sizeof(normals) / sizeof(float),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float)
    );
    indicesSize = sizeof(indices);

    quadData = computeGPUReadyBuffer(&quadDataSize,
        verticesQuad, sizeof(verticesQuad) / sizeof(float),
        normalsQuad, sizeof(normalsQuad) / sizeof(float),
        textureCoordinatesQuad, sizeof(textureCoordinatesQuad) / sizeof(float)
    );
    indicesQuadSize = sizeof(indicesQuad);

    prMeshData* meshCube = prMeshCreate();
    prMeshLinkContext(meshCube, test->openglContext);
    prMeshUpdate(meshCube,
        cubeData, cubeDataSize,
        indices, indicesSize);

    prMeshData* meshQuad = prMeshCreate();
    prMeshLinkContext(meshQuad, test->openglContext);
    prMeshUpdate(meshQuad,
        quadData, quadDataSize,
        indicesQuad, indicesQuadSize);

    typedef struct directionalLightData {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    } directionalLightData;

    directionalLightData sun = {
        {-0.25f, -0.5f, -0.75f},
        {0.095f, 0.095f, 0.1f},
        {0.8f, 0.8f, 0.75f},
        {0.9f, 0.9f, 0.85f}
    };

    typedef struct pointLightData {
        float constant;
        float linear;
        float quadratic;

        vec3 position;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    } pointLightData;

    pointLightData point = {
        1.0f,
        0.09f,
        0.032f,
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f}
    };

    prShaderSetUniform3f(shaderProgram, "directionalLights[0].direction", sun.direction[0], sun.direction[1], sun.direction[2]);
    prShaderSetUniform3f(shaderProgram, "directionalLights[0].ambient", sun.ambient[0], sun.ambient[1], sun.ambient[2]);
    prShaderSetUniform3f(shaderProgram, "directionalLights[0].diffuse", sun.diffuse[0], sun.diffuse[1], sun.diffuse[2]);
    prShaderSetUniform3f(shaderProgram, "directionalLights[0].specular", sun.specular[0], sun.specular[1], sun.specular[2]);

    prShaderSetUniform1f(shaderProgram, "pointLights[0].constant", point.constant);
    prShaderSetUniform1f(shaderProgram, "pointLights[0].linear", point.linear);
    prShaderSetUniform1f(shaderProgram, "pointLights[0].quadratic", point.quadratic);

    prShaderSetUniform3f(shaderProgram, "pointLights[0].position", point.position[0], point.position[1], point.position[2]);
    prShaderSetUniform3f(shaderProgram, "pointLights[0].ambient", point.ambient[0], point.ambient[1], point.ambient[2]);
    prShaderSetUniform3f(shaderProgram, "pointLights[0].diffuse", point.diffuse[0], point.diffuse[1], point.diffuse[2]);
    prShaderSetUniform3f(shaderProgram, "pointLights[0].specular", point.specular[0], point.specular[1], point.specular[2]);

    camera = prCameraCreate();
    prCameraLinkContext(camera, test->openglContext);

    test->openglContext->Enable(GL_DEPTH_TEST);
    test->openglContext->ClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    test->openglContext->Enable(GL_CULL_FACE);
    test->openglContext->Enable(GL_BLEND);

    glfwMaximizeWindow(test->window);
    while(!glfwWindowShouldClose(test->window)) {
        test->openglContext->Enable(GL_DEPTH_TEST);
        prFramebufferClear(test->openglContext, NULL, PR_BUFFER_BIT_COLOR | PR_BUFFER_BIT_DEPTH);
        prFramebufferClear(test->openglContext, framebuffer, PR_BUFFER_BIT_COLOR | PR_BUFFER_BIT_DEPTH);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        mat4 translation;

        vec3 rotation = {radians(yaw), radians(pitch), radians(0.0f)};
        prCameraUpdate(camera, cameraPosition, rotation, 45.0f, 0.1f, 1500.0f);

        prShaderSetUniform3f(shaderProgram, "cameraPosition", camera->position[0], camera->position[1], camera->position[2]);
        prShaderSetUniformMatrix4fv(shaderProgram, "view", camera->view[0]);
        prShaderSetUniformMatrix4fv(shaderProgram, "projection", camera->projection[0]);

        float val = smoothOvertimeSin();

        prFramebufferBind(framebuffer);

        translationsToMatrix(translation, (vec3){0.0f, 0.0f, -20.0f}, GLM_VEC3_ZERO, (vec3){30.0f, 30.0f, 10.0f});
        bindMaterial(&materialMetal, shaderProgram);
        prShaderSetUniformMatrix4fv(shaderProgram, "translation", translation[0]);
        prMeshDraw(meshCube);

        translationsToMatrix(translation, (vec3){0.0f, -20.0f, 0.0f}, GLM_VEC3_ZERO, (vec3){30.0f, 10.0f, 30.0f});
        bindMaterial(&materialMetal, shaderProgram);
        prShaderSetUniformMatrix4fv(shaderProgram, "translation", translation[0]);
        prMeshDraw(meshCube);

        translationsToMatrix(translation, (vec3){-20.0f, 0.0f, 0.0f}, GLM_VEC3_ZERO, (vec3){10.0f, 30.0f, 30.0f});
        bindMaterial(&materialMetal, shaderProgram);
        prShaderSetUniformMatrix4fv(shaderProgram, "translation", translation[0]);
        prMeshDraw(meshCube);

        translationsToMatrix(translation, (vec3){1.0f, 0.0f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        bindMaterial(&materialWood, shaderProgram);
        prShaderSetUniformMatrix4fv(shaderProgram, "translation", translation[0]);
        prMeshDraw(meshCube);

        translationsToMatrix(translation, (vec3){-1.0f, 0.0f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        bindMaterial(&materialWoodMetal, shaderProgram);
        prShaderSetUniformMatrix4fv(shaderProgram, "translation", translation[0]);
        prMeshDraw(meshCube);

        translationsToMatrix(translation, (vec3){0.0f, 1.5f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        bindMaterial(&materialBrick, shaderProgram);
        prShaderSetUniformMatrix4fv(shaderProgram, "translation", translation[0]);
        prMeshDraw(meshCube);

        translationsToMatrix(translation, (vec3){0.0f, val / 3.5f - 1.5f, 0.0f}, (vec3){0.0f, radians(val * 100.0f), 0.0f}, GLM_VEC3_ONE);
        bindMaterial(&defaultMaterial, shaderProgram);
        prShaderSetUniformMatrix4fv(shaderProgram, "translation", translation[0]);
        prMeshDraw(meshCube);

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
        test->openglContext->Disable(GL_CULL_FACE);
        test->openglContext->DepthFunc(GL_LEQUAL);
        prShaderBind(skyboxShaderProgram);
        prMeshDraw(meshCube);
        test->openglContext->Enable(GL_CULL_FACE);

        if(showHUD == 1) {
            test->openglContext->Disable(GL_DEPTH_TEST);
            bindMaterialHUD(&materialHUD, hudShaderProgram);
            prMeshDraw(meshQuad);
        }

        prFramebufferUnbind(test->openglContext);

        prFramebufferBlit(test->openglContext, framebuffer, NULL,
            0, 0, windowWidth, windowHeight,
            0, 0, windowWidth, windowHeight,
            PR_BUFFER_BIT_COLOR, PR_FILTER_NEAREST
        );

        if(showPostProcessing) {
            test->openglContext->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            prTextureBindTexture(colorTexture, 0);
            prTextureBindImage(postProcessingTexture, 1, 0, PR_ACCESS_WRITE_ONLY, GL_RGBA32F);
            prComputeShaderDispatch(computeShaderProgram, windowWidth , windowHeight, 1);
            test->openglContext->MemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

            bindMaterialHUD(&materialPostProcessing, hudShaderProgram);
            prMeshDraw(meshQuad);
        }

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
    prTextureDestroy(brickWallSpecularTexture);
    brickWallSpecularTexture = NULL;
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
    prShaderDestroy(hudShaderProgram);
    hudShaderProgram = NULL;
    prShaderDestroy(skyboxShaderProgram);
    skyboxShaderProgram = NULL;
    prShaderDestroy(shaderProgram);
    shaderProgram = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    fclose(logFile);
    logFile = NULL;
}
