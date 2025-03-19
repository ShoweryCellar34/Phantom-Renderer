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

    prWindow* test = prWindowCreate("Test", 800, 450);
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

    prTextureData* containerMetalSpecularTexture = prTextureCreate();
    loadTexture(containerMetalSpecularTexture, test->openglContext, "res/container2_specular.png");

    prTextureData* steelTexture = prTextureCreate();
    loadTexture(steelTexture, test->openglContext, "res/steel.jpg");

    prTextureData* blackTexture = prTextureCreate();
    prTextureLinkContext(blackTexture, test->openglContext);
    prTextureSingleColor(blackTexture, (float[4]){0.0f, 0.0f, 0.0f, 0.0f});

    prTextureData* whiteTexture = prTextureCreate();
    prTextureLinkContext(whiteTexture, test->openglContext);
    prTextureSingleColor(whiteTexture, (float[4]){1.0f, 1.0f, 1.0f, 0.0f});

    prMaterialData* materialMetal = prMaterialCreate();
    prMaterialLinkAmbientMap(materialMetal, steelTexture);
    prMaterialLinkDiffuseMap(materialMetal, steelTexture);
    prMaterialLinkSpecularMap(materialMetal, whiteTexture);
    materialMetal->shininess = 256.0f;

    prMaterialData* materialWood = prMaterialCreate();
    prMaterialLinkAmbientMap(materialWood, containerTexture);
    prMaterialLinkDiffuseMap(materialWood, containerTexture);
    prMaterialLinkSpecularMap(materialWood, blackTexture);
    prMaterialSetShininess(materialWood, 32.0f);

    prMaterialData* materialWoodMetal = prMaterialCreate();
    prMaterialLinkAmbientMap(materialWoodMetal, containerMetalTexture);
    prMaterialLinkDiffuseMap(materialWoodMetal, containerMetalTexture);
    prMaterialLinkSpecularMap(materialWoodMetal, containerMetalSpecularTexture);
    prMaterialSetShininess(materialWoodMetal, 256.0f);

    prMeshData* meshMetal = prMeshCreate();
    prMeshLinkContext(meshMetal, test->openglContext);
    prMeshUpdate(meshMetal, vertices, sizeof(vertices) / sizeof(float), 
        indices, sizeof(indices) / sizeof(unsigned int), 
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float), 
        NULL, 0, 
        normals, sizeof(normals) / sizeof(float));
    prMeshLinkMaterial(meshMetal, materialMetal);

    prMeshData* meshWood = prMeshCreate();
    prMeshLinkContext(meshWood, test->openglContext);
    prMeshUpdate(meshWood, vertices, sizeof(vertices) / sizeof(float), 
        indices, sizeof(indices) / sizeof(unsigned int), 
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float), 
        NULL, 0, 
        normals, sizeof(normals) / sizeof(float));
    prMeshLinkMaterial(meshWood, materialWood);

    prMeshData* meshWoodMetal = prMeshCreate();
    prMeshLinkContext(meshWoodMetal, test->openglContext);
    prMeshUpdate(meshWoodMetal, vertices, sizeof(vertices) / sizeof(float), 
        indices, sizeof(indices) / sizeof(unsigned int), 
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float), 
        NULL, 0, 
        normals, sizeof(normals) / sizeof(float));
    prMeshLinkMaterial(meshWoodMetal, materialWoodMetal);

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

        translationsToMatrix(translation, (vec3){0.0f, 0.0f, -3.0f}, GLM_VEC3_ZERO, (vec3){10.0f, 10.0f, 0.5f});
        prMeshDraw(meshMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){1.0f, 0.0f, 0.0f}, (vec3){0.0f, cos((glfwGetTimerValue() * 0.5f) / glfwGetTimerFrequency()), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshWood, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){-1.0f, 0.0f, 0.0f}, (vec3){0.0f, sin((glfwGetTimerValue() * 0.5f) / glfwGetTimerFrequency()), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshWoodMetal, translation, camera, shaderProgram);

        glfwSwapBuffers(test->window);
        glfwPollEvents();
        proccessInput(test->window, camera->front, camera->up);
    }

    prCameraDestroy(camera);
    camera = NULL;

    prMeshDestroy(meshWoodMetal);
    meshWoodMetal = NULL;
    prMeshDestroy(meshWood);
    meshWood = NULL;
    prMeshDestroy(meshMetal);
    meshMetal = NULL;

    prMaterialDestroy(materialWoodMetal);
    materialWoodMetal = NULL;
    prMaterialDestroy(materialWood);
    materialWood = NULL;
    prMaterialDestroy(materialMetal);
    materialMetal = NULL;

    prTextureDestroy(whiteTexture);
    whiteTexture = NULL;
    prTextureDestroy(blackTexture);
    blackTexture = NULL;
    prTextureDestroy(steelTexture);
    steelTexture = NULL;
    prTextureDestroy(containerMetalSpecularTexture);
    containerMetalSpecularTexture = NULL;
    prTextureDestroy(containerMetalTexture);
    containerMetalTexture = NULL;
    prTextureDestroy(containerTexture);
    containerTexture = NULL;
    prTextureDestroy(faceTexture);
    faceTexture = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    prEndFileLogging();
}
