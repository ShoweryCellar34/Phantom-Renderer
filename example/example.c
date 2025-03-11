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

    prWindow* test = prWindowCreate("Test", 600, 600);
    prWindowInitContext(test);
    glfwSetWindowUserPointer(test->window, test->openglContext);

    prEnableImageFlip();

    unsigned int shaderProgram = prShaderGenerateDefaultProgram(test->openglContext);

    prTextureData* testTexture = prTextureCreate();
    loadTexture(testTexture, test->openglContext, "res/awesomeface.png");

    prTextureData* testTexture2 = prTextureCreate();
    loadTexture(testTexture2, test->openglContext, "res/container.jpg");

    prMeshData* testMesh = prMeshCreate();
    prMeshLinkWindow(testMesh, test->openglContext);
    prMeshLinkTexture(testMesh, testTexture);
    prMeshUpdate(testMesh, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float), vertexColor, sizeof(vertexColor) / sizeof(float));
    prMeshTextureToColorRatio(testMesh, 0.5f);

    prMeshData* testMesh2 = prMeshCreate();
    prMeshLinkWindow(testMesh2, test->openglContext);
    prMeshLinkTexture(testMesh2, testTexture2);
    prMeshUpdate(testMesh2, vertices2, sizeof(vertices2) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float), NULL, 0);
    prMeshTextureToColorRatio(testMesh2, 1.0f);

    prMeshData* testMesh3 = prMeshCreate();
    prMeshLinkWindow(testMesh3, test->openglContext);
    prMeshUpdate(testMesh3, vertices3, sizeof(vertices3) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), NULL, 0, vertexColor, sizeof(vertexColor) / sizeof(float));
    prMeshTextureToColorRatio(testMesh3, 0.0f);

    prCamera* camera = prCameraCreate();
    prCameraLink(camera, test->openglContext);

    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);
    glfwSetKeyCallback(test->window, keyCallback);

    while(!glfwWindowShouldClose(test->window)) {
        prWindowClear(test->openglContext);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        mat4 translation;
        glm_mat4_identity(translation);
        vec3 offset;
        offset[0] = 0.0f;
        offset[1] = 0.0f;
        offset[2] = -3.0f;
        glm_translate(translation, offset);

        prCameraUpdate(camera, cameraPosition);

        test->openglContext->Enable(GL_DEPTH_TEST);
        test->openglContext->Enable(GL_BLEND);
        test->openglContext->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        prMeshDraw(testMesh3, translation, camera, shaderProgram);
        prMeshDraw(testMesh2, translation, camera, shaderProgram);
        prMeshDraw(testMesh, translation, camera, shaderProgram);

        test->openglContext->Disable(GL_BLEND);
        test->openglContext->Disable(GL_DEPTH);

        glfwSwapBuffers(test->window);

        glfwPollEvents();
    }

    prCameraDestroy(camera);
    camera = NULL;

    prMeshDestroy(testMesh);
    testMesh = NULL;
    prMeshDestroy(testMesh2);
    testMesh2 = NULL;
    prMeshDestroy(testMesh3);
    testMesh3 = NULL;

    prTextureDestroy(testTexture);
    testTexture = NULL;
    prTextureDestroy(testTexture2);
    testTexture2 = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    prEndFileLogging();
}
