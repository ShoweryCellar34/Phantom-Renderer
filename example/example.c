#include <PR/PR.h>

#include <stdio.h>
#include <cglm/cglm.h>
#include <PR/memory.h>
#include "exampleGlobalValues.h"
#include "exampleFunctions.h"

int main(int argc, char** argv) {
    prSetLogLevel(PR_LOG_LEVEL_TRACE);
    prStartFileLogging();

    glfwInit();

    prWindow* test = prWindowCreate("Test", 1600, 900);
    prWindowInitContext(test);
    glfwSetWindowUserPointer(test->window, test->openglContext);

    prEnableImageFlip();

    unsigned int shaderProgram = prShaderGenerateDefaultProgram(test->openglContext);

    prTextureData* faceTexture = prTextureCreate();
    loadTexture(faceTexture, test->openglContext, "res/awesomeface.png");

    prTextureData* containerTexture = prTextureCreate();
    loadTexture(containerTexture, test->openglContext, "res/container.jpg");

    prTextureData* containerMetalTexture = prTextureCreate();
    loadTexture(containerMetalTexture, test->openglContext, "res/container2.png");

    prTextureData* steelTexture = prTextureCreate();
    loadTexture(steelTexture, test->openglContext, "res/steel.jpg");

    prMaterialData* materialMetal = prMaterialCreateDefaults();
    prMaterialLinkTexture(materialMetal, steelTexture);
    prMaterialLinkAmbientMap(materialMetal, steelTexture);
    prMaterialLinkDiffuseMap(materialMetal, steelTexture);
    prMaterialLinkSpecularMap(materialMetal, steelTexture);
    materialMetal->specularStrength = 1.0f;
    materialMetal->shininess = 256.0f;
    materialMetal->diffuseStrength = 2.5f;
    materialMetal->ambientStrength = 0.4f;

    prMaterialData* materialWood = prMaterialCreateDefaults();
    prMaterialLinkTexture(materialWood, containerTexture);
    prMaterialLinkAmbientMap(materialWood, containerTexture);
    prMaterialLinkDiffuseMap(materialWood, containerTexture);
    prMaterialLinkSpecularMap(materialWood, containerTexture);
    materialWood->specularStrength = 0.2f;
    materialWood->ambientStrength = 0.3f;

    prMaterialData* materialWoodMetal = prMaterialCreateDefaults();
    prMaterialLinkTexture(materialWoodMetal, containerMetalTexture);
    prMaterialLinkAmbientMap(materialWoodMetal, containerMetalTexture);
    prMaterialLinkDiffuseMap(materialWoodMetal, containerMetalTexture);
    prMaterialLinkSpecularMap(materialWoodMetal, containerMetalTexture);
    materialWoodMetal->specularStrength = 0.2f;
    materialWoodMetal->ambientStrength = 0.3f;

    prMeshData* testMeshMetal = prMeshCreate();
    prMeshLinkContext(testMeshMetal, test->openglContext);
    prMeshUpdate(testMeshMetal, vertices2, sizeof(vertices2) / sizeof(float), 
        indices2, sizeof(indices2) / sizeof(unsigned int), 
        textureCoordinates2, sizeof(textureCoordinates2) / sizeof(float), 
        NULL, 0, 
        normals2, sizeof(normals2) / sizeof(float));
    prMeshTextureToColorRatio(testMeshMetal, 1.0f);
    prMeshLinkMaterial(testMeshMetal, materialMetal);

    prMeshData* testMeshWood = prMeshCreate();
    prMeshLinkContext(testMeshWood, test->openglContext);
    prMeshUpdate(testMeshWood, vertices2, sizeof(vertices2) / sizeof(float), 
        indices2, sizeof(indices2) / sizeof(unsigned int), 
        textureCoordinates2, sizeof(textureCoordinates2) / sizeof(float), 
        NULL, 0, 
        normals2, sizeof(normals2) / sizeof(float));
    prMeshTextureToColorRatio(testMeshWood, 1.0f);
    prMeshLinkMaterial(testMeshWood, materialWood);

    prMeshData* testMeshWoodMetal = prMeshCreate();
    prMeshLinkContext(testMeshWoodMetal, test->openglContext);
    prMeshUpdate(testMeshWoodMetal, vertices2, sizeof(vertices2) / sizeof(float), 
        indices2, sizeof(indices2) / sizeof(unsigned int), 
        textureCoordinates2, sizeof(textureCoordinates2) / sizeof(float), 
        NULL, 0, 
        normals2, sizeof(normals2) / sizeof(float));
    prMeshTextureToColorRatio(testMeshWoodMetal, 1.0f);
    prMeshLinkMaterial(testMeshWoodMetal, materialWoodMetal);

    prCamera* camera = prCameraCreate();
    prCameraLinkContext(camera, test->openglContext);

    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);
    glfwSetCursorPosCallback(test->window, cursorPosCallback);
    glfwSetInputMode(test->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    test->openglContext->Enable(GL_DEPTH_TEST);
    test->openglContext->Enable(GL_BLEND);
    test->openglContext->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(test->window)) {
        prWindowClear(test->openglContext);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        mat4 translation;

        vec3 rotation = {yaw, pitch, 0.0f};
        prCameraUpdate(camera, cameraPosition, rotation, 45.0f, 0.001f, 10000.0f);

        translationsToMatrix(translation, (vec3){1.5f, 0.0f, 0.0f}, (vec3){0.0f, sin((glfwGetTimerValue() * 0.5f) / glfwGetTimerFrequency()), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(testMeshMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, cos((glfwGetTimerValue() * 0.5f) / glfwGetTimerFrequency()), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(testMeshWood, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){-1.5f, 0.0f, 0.0f}, (vec3){0.0f, sin((glfwGetTimerValue() * 0.5f) / glfwGetTimerFrequency()), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(testMeshWoodMetal, translation, camera, shaderProgram);

        glfwSwapBuffers(test->window);
        glfwPollEvents();
        proccessInput(test->window, camera->front, camera->up);
    }

    prCameraDestroy(camera);
    camera = NULL;

    prMeshDestroy(testMeshMetal);
    testMeshMetal = NULL;
    prMeshDestroy(testMeshWood);
    testMeshWood = NULL;

    prMaterialDestroy(materialMetal);
    materialMetal = NULL;
    prMaterialDestroy(materialWood);
    materialWood = NULL;
    prMaterialDestroy(materialWoodMetal);
    materialWoodMetal = NULL;

    prTextureDestroy(faceTexture);
    faceTexture = NULL;
    prTextureDestroy(containerTexture);
    containerTexture = NULL;
    prTextureDestroy(containerMetalTexture);
    containerMetalTexture = NULL;
    prTextureDestroy(steelTexture);
    steelTexture = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    prEndFileLogging();
}
