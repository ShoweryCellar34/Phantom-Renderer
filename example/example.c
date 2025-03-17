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

    prTextureData* testTexture = prTextureCreate();
    loadTexture(testTexture, test->openglContext, "res/awesomeface.png");

    prTextureData* testTexture2 = prTextureCreate();
    loadTexture(testTexture2, test->openglContext, "res/container.jpg");

    prMeshData* testMeshMixed = prMeshCreate();
    prMeshLinkContext(testMeshMixed, test->openglContext);
    prMeshLinkTexture(testMeshMixed, testTexture);
    prMeshUpdate(testMeshMixed, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float), vertexColor, sizeof(vertexColor) / sizeof(float));
    prMeshTextureToColorRatio(testMeshMixed, 0.5f);

    prMeshData* testMeshTexture = prMeshCreate();
    prMeshLinkContext(testMeshTexture, test->openglContext);
    prMeshLinkTexture(testMeshTexture, testTexture2);
    prMeshUpdate(testMeshTexture, vertices2, sizeof(vertices2) / sizeof(float), indices2, sizeof(indices2) / sizeof(unsigned int), textureCoordinates2, sizeof(textureCoordinates2) / sizeof(float), NULL, 0);
    prMeshTextureToColorRatio(testMeshTexture, 1.0f);

    prMeshData* testMeshColor = prMeshCreate();
    prMeshLinkContext(testMeshColor, test->openglContext);
    prMeshUpdate(testMeshColor, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), NULL, 0, vertexColor, sizeof(vertexColor) / sizeof(float));
    prMeshTextureToColorRatio(testMeshColor, 0.0f);

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

        translationsToMatrix(translation, (vec3){1.5f, 0.0f, 0.0f}, GLM_VEC3_ZERO, GLM_VEC3_ONE);
        prMeshDraw(testMeshTexture, translation, camera, shaderProgram);

        translationsToMatrix(translation, GLM_VEC3_ZERO, GLM_VEC3_ZERO, GLM_VEC3_ONE);
        prMeshDraw(testMeshMixed, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){-1.5f, 0.0f, 0.0f}, GLM_VEC3_ZERO, GLM_VEC3_ONE);
        prMeshDraw(testMeshColor, translation, camera, shaderProgram);

        glfwSwapBuffers(test->window);
        glfwPollEvents();
        proccessInput(test->window, camera->front, camera->up);
    }

    prCameraDestroy(camera);
    camera = NULL;

    prMeshDestroy(testMeshMixed);
    testMeshMixed = NULL;
    prMeshDestroy(testMeshTexture);
    testMeshTexture = NULL;
    prMeshDestroy(testMeshColor);
    testMeshColor = NULL;

    prTextureDestroy(testTexture);
    testTexture = NULL;
    prTextureDestroy(testTexture2);
    testTexture2 = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    prEndFileLogging();
}
