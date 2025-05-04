#include <PR/PR.h>
#include <PR/helperStuffs.h>

#include <stdio.h>
#include <cglm/cglm.h>
#include <PR/memory.h>
#include "exampleGlobalValues.h"
#include "exampleFunctions.h"

int main(int argc, char** argv) {
    prLogSetLevel(PR_LOG_TRACE);
    FILE* logFile = fopen("prLog.txt", "w");
    prLogSetStream(2, (FILE*[]){stdout, logFile});

    glfwInit();

    prWindow* test = prWindowCreate(TITLE, windowWidth, windowHeight);
    prWindowInitContext(test);
    glfwSetWindowUserPointer(test->window, test->openglContext);

    prEnableImageFlip();

    prShaderData* shaderProgram = loadDefaultShader(test->openglContext);

    prShaderData* hudShaderProgram = prShaderCreate();
    prShaderLinkContext(hudShaderProgram, test->openglContext);
    prShaderUpdate(hudShaderProgram, 0, 0, HUD_VERTEX_SHADER, HUD_FRAGMENT_SHADER);

    prComputeShaderData* computeShaderProgram = prComputeShaderCreate();
    prComputeShaderLinkContext(computeShaderProgram, test->openglContext);
    prComputeShaderUpdate(computeShaderProgram, HUD_COMPUTE_SHADER);

    prTextureData* defaultTexture = makeTextureCheckerboard(test->openglContext, 8, (float[4]){1.0f, 0.0f, 1.0f, 1.0f}, (float[4]){0.0f, 0.0f, 0.0f, 0.0f});

    prTextureData* containerTexture = loadTexture(test->openglContext, "res/container.jpg");

    prTextureData* containerMetalTexture = loadTexture(test->openglContext, "res/container2.png");

    prTextureData* containerMetalSpecularTexture = loadTexture(test->openglContext, "res/container2_specular.png");

    prTextureData* steelTexture = loadTexture(test->openglContext, "res/steel.jpg");

    prTextureData* steelNormal = loadTexture(test->openglContext, "res/steelNormal.png");

    prTextureData* brickWallDiffuseTexture = loadTexture(test->openglContext, "res/brickWallDiffuse.tga");

    prTextureData* brickWallNormalTexture = loadTexture(test->openglContext, "res/brickWallNormal.tga");

    prTextureData* brickWallSpecularTexture = loadTexture(test->openglContext, "res/brickWallSpecular.png");

    prTextureData* blackTexture = makeTextureSingleColor(test->openglContext, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});

    prTextureData* whiteTexture = makeTextureSingleColor(test->openglContext, (float[4]){1.0f, 1.0f, 1.0f, 1.0f});

    prTextureData* defaultNormal = makeTextureSingleColor(test->openglContext, (float[4]){0.0f, -1.0f, 0.0f, 1.0f});

    colorTexture = prTextureCreate();
    prTextureLinkContext(colorTexture, test->openglContext);
    prTextureUpdate(colorTexture, PR_FORMAT_RGBA, PR_FILTER_LINEAR, PR_WRAPPING_EDGE, NULL, 0, windowWidth, windowHeight);

    depthStencilRBO = prRenderBufferCreate();
    prRenderBufferLinkContext(depthStencilRBO, test->openglContext);
    prRenderBufferUpdate(depthStencilRBO, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight);

    framebuffer = prFramebufferCreate();
    prFramebufferLinkContext(framebuffer, test->openglContext);
    prFramebufferLinkColorTexture(framebuffer, colorTexture);
    prFramebufferLinkDepthStencilTextureRBO(framebuffer, depthStencilRBO);

    prMaterialData* defaultMaterial = makeMaterialOneTexture(defaultTexture);
    defaultMaterial->shininess = 48.0f;

    prMaterialData* materialWhite = makeMaterialOneTexture(whiteTexture);
    materialWhite->shininess = 0.0f;

    prMaterialData* materialMetal = prMaterialCreate();
    prMaterialLinkAmbientMap(materialMetal, steelTexture);
    prMaterialLinkDiffuseMap(materialMetal, steelTexture);
    prMaterialLinkSpecularMap(materialMetal, whiteTexture);
    prMaterialLinkNormalMap(materialMetal, steelNormal);
    materialMetal->shininess = 256.0f;

    prMaterialData* materialWood = prMaterialCreate();
    prMaterialLinkAmbientMap(materialWood, containerTexture);
    prMaterialLinkDiffuseMap(materialWood, containerTexture);
    prMaterialLinkSpecularMap(materialWood, blackTexture);
    prMaterialLinkNormalMap(materialWood, whiteTexture);
    prMaterialSetShininess(materialWood, 32.0f);

    prMaterialData* materialWoodMetal = prMaterialCreate();
    prMaterialLinkAmbientMap(materialWoodMetal, containerMetalTexture);
    prMaterialLinkDiffuseMap(materialWoodMetal, containerMetalTexture);
    prMaterialLinkSpecularMap(materialWoodMetal, containerMetalSpecularTexture);
    prMaterialLinkNormalMap(materialWoodMetal, whiteTexture);
    prMaterialSetShininess(materialWoodMetal, 256.0f);

    prMaterialData* materialBrick = prMaterialCreate();
    prMaterialLinkAmbientMap(materialBrick, brickWallDiffuseTexture);
    prMaterialLinkDiffuseMap(materialBrick, brickWallDiffuseTexture);
    prMaterialLinkSpecularMap(materialBrick, brickWallSpecularTexture);
    prMaterialLinkNormalMap(materialBrick, brickWallNormalTexture);
    prMaterialSetShininess(materialBrick, 32.0f);

    prMaterialData* materialQuad = prMaterialCreate();
    prMaterialLinkAmbientMap(materialQuad, colorTexture);
    prMaterialLinkDiffuseMap(materialQuad, blackTexture);
    prMaterialLinkSpecularMap(materialQuad, blackTexture);
    prMaterialLinkNormalMap(materialQuad, blackTexture);
    prMaterialSetShininess(materialQuad, 0.0f);

    prMeshData* meshMetal = prMeshCreate();
    prMeshLinkContext(meshMetal, test->openglContext);
    prMeshUpdate(meshMetal,
        vertices, sizeof(vertices) / sizeof(float),
        normals, sizeof(normals) / sizeof(float),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int));
    prMeshLinkMaterial(meshMetal, materialMetal);

    prMeshData* meshWood = prMeshCreate();
    prMeshLinkContext(meshWood, test->openglContext);
    prMeshUpdate(meshWood,
        vertices, sizeof(vertices) / sizeof(float),
        normals, sizeof(normals) / sizeof(float),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int));
    prMeshLinkMaterial(meshWood, materialWood);

    prMeshData* meshWoodMetal = prMeshCreate();
    prMeshLinkContext(meshWoodMetal, test->openglContext);
    prMeshUpdate(meshWoodMetal,
        vertices, sizeof(vertices) / sizeof(float),
        normals, sizeof(normals) / sizeof(float),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int));
    prMeshLinkMaterial(meshWoodMetal, materialWoodMetal);

    prMeshData* meshBrick = prMeshCreate();
    prMeshLinkContext(meshBrick, test->openglContext);
    prMeshUpdate(meshBrick,
        vertices, sizeof(vertices) / sizeof(float),
        normals, sizeof(normals) / sizeof(float),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int));
    prMeshLinkMaterial(meshBrick, materialBrick);

    prMeshData* meshItem = prMeshCreate();
    prMeshLinkContext(meshItem, test->openglContext);
    prMeshUpdate(meshItem,
        vertices, sizeof(vertices) / sizeof(float),
        normals, sizeof(normals) / sizeof(float),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int));
    prMeshLinkMaterial(meshItem, defaultMaterial);

    prMeshData* meshQuad = prMeshCreate();
    prMeshLinkContext(meshQuad, test->openglContext);
    prMeshUpdate(meshQuad,
        verticesQuad, sizeof(verticesQuad) / sizeof(float),
        normalsQuad, sizeof(normalsQuad) / sizeof(float),
        textureCoordinatesQuad, sizeof(textureCoordinatesQuad) / sizeof(float),
        indicesQuad, sizeof(indicesQuad) / sizeof(unsigned int));
    prMeshLinkMaterial(meshQuad, materialQuad);

    prDirectionalLightData* sun = prDirectionalLightCreate();
    prDirectionalLightSetDirection(sun, (vec3){-1.0f, -1.0f, -1.0f});
    prDirectionalLightSetAmbient(sun, (vec3){0.095f, 0.095f, 0.1f});
    prDirectionalLightSetDiffuse(sun, (vec3){0.8f, 0.8f, 0.75f});
    prDirectionalLightSetSpecular(sun, (vec3){0.9f, 0.9f, 0.85f});

    prPointLightData* point = prPointLightCreate();
    point->constant = 1.0;
    point->linear = 0.7f;
    point->quadratic = 1.8f;
    prPointLightSetPosition(point, (vec3){0.0f, 0.0f, 0.0f});
    prPointLightSetDiffuse(point, (vec3){0.6f, 0.6f, 1.0f});

    prShaderUniform3f(shaderProgram, "directionalLights[0].direction", sun->direction[0], sun->direction[1], sun->direction[2]);
    prShaderUniform3f(shaderProgram, "directionalLights[0].ambient", sun->ambient[0], sun->ambient[1], sun->ambient[2]);
    prShaderUniform3f(shaderProgram, "directionalLights[0].diffuse", sun->diffuse[0], sun->diffuse[1], sun->diffuse[2]);
    prShaderUniform3f(shaderProgram, "directionalLights[0].specular", sun->specular[0], sun->specular[1], sun->specular[2]);

    prShaderUniform1f(shaderProgram, "pointLights[0].constant", point->constant);
    prShaderUniform1f(shaderProgram, "pointLights[0].linear", point->linear);
    prShaderUniform1f(shaderProgram, "pointLights[0].quadratic", point->quadratic);

    prShaderUniform3f(shaderProgram, "pointLights[0].position", point->position[0], point->position[1], point->position[2]);
    prShaderUniform3f(shaderProgram, "pointLights[0].ambient", point->ambient[0], point->ambient[1], point->ambient[2]);
    prShaderUniform3f(shaderProgram, "pointLights[0].diffuse", point->diffuse[0], point->diffuse[1], point->diffuse[2]);
    prShaderUniform3f(shaderProgram, "pointLights[0].specular", point->specular[0], point->specular[1], point->specular[2]);

    camera = prCameraCreate();
    prCameraLinkContext(camera, test->openglContext);

    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);
    glfwSetCursorPosCallback(test->window, cursorPosCallback);
    glfwSetInputMode(test->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    test->openglContext->Enable(GL_DEPTH_TEST);
    test->openglContext->ClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    test->openglContext->Enable(GL_CULL_FACE);
    test->openglContext->Enable(GL_BLEND);
    test->openglContext->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(test->window)) {
        test->openglContext->UseProgram(computeShaderProgram->computeShaderProgramObject);
        prTextureBindImage(colorTexture, 0, 0, PR_ACCESS_WRITE_ONLY, GL_RGBA32F);
        prComputeShaderDispatch(computeShaderProgram, windowWidth, windowHeight, 1);
        test->openglContext->MemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        test->openglContext->Enable(GL_DEPTH_TEST);
        test->openglContext->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        mat4 translation;

        vec3 rotation = {radians(yaw), radians(pitch), radians(0.0f)};
        prCameraUpdate(camera, cameraPosition, rotation, 45.0f, 0.1f, 100.0f);

        float val = smoothOvertimeSin();

        translationsToMatrix(translation, (vec3){0.0f, 0.0f, -20.0f}, GLM_VEC3_ZERO, (vec3){30.0f, 30.0f, 10.0f});
        prMeshDraw(meshMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){0.0f, -20.0f, 0.0f}, GLM_VEC3_ZERO, (vec3){30.0f, 10.0f, 30.0f});
        prMeshDraw(meshMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){1.0f, 0.0f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshWood, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){-1.0f, 0.0f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshWoodMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){0.0f, 1.5f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshBrick, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){0.0f, val / 3.5f - 1.5f, 0.0f}, (vec3){0.0f, radians(val * 100.0f), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshItem, translation, camera, shaderProgram);

        test->openglContext->Disable(GL_DEPTH_TEST);
        translationsToMatrix(translation, (vec3){0.0f, 0.0f, 0.0f}, GLM_VEC3_ZERO, GLM_VEC3_ONE);
        prMeshDraw(meshQuad, translation, camera, hudShaderProgram);

        // prFramebufferBlit(test->openglContext, framebuffer, NULL, 0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, PR_BUFFER_BIT_COLOR, GL_NEAREST);

        glfwSwapBuffers(test->window);
        glfwPollEvents();
        proccessInput(test->window);
    }

    prCameraDestroy(camera);
    camera = NULL;

    prMeshDestroy(meshQuad);
    meshQuad = NULL;
    prMeshDestroy(meshWoodMetal);
    meshWoodMetal = NULL;
    prMeshDestroy(meshWood);
    meshWood = NULL;
    prMeshDestroy(meshMetal);
    meshMetal = NULL;

    prMaterialDestroy(materialQuad);
    materialQuad = NULL;
    prMaterialDestroy(materialWoodMetal);
    materialWoodMetal = NULL;
    prMaterialDestroy(materialWood);
    materialWood = NULL;
    prMaterialDestroy(materialMetal);
    materialMetal = NULL;
    prMaterialDestroy(materialWhite);
    materialWhite = NULL;
    prMaterialDestroy(defaultMaterial);
    defaultMaterial = NULL;

    prFramebufferDestroy(framebuffer);
    framebuffer = NULL;
    prRenderBufferDestroy(depthStencilRBO);
    depthStencilRBO = NULL;
    prTextureDestroy(colorTexture);
    colorTexture = NULL;

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
    prShaderDestroy(shaderProgram);
    shaderProgram = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    fclose(logFile);
    logFile = NULL;
}
