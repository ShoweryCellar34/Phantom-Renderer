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

    prWindow* test = prWindowCreate("Test", 1280, 720);
    prWindowInitContext(test);
    glfwSetWindowUserPointer(test->window, test->openglContext);

    prEnableImageFlip();

    prShaderProgramData* shaderProgram = prShaderProgramCreate();
    prShaderProgramLinkContext(shaderProgram, test->openglContext);
    prShaderProgramUpdate(shaderProgram, vertexShader, fragmentShader);

    prTextureData* defaultTexture = makeTextureDefault(test->openglContext, 8);

    prTextureData* containerTexture = loadTexture(test->openglContext, "res/container.jpg");

    prTextureData* containerMetalTexture = loadTexture(test->openglContext, "res/container2.png");

    prTextureData* containerMetalSpecularTexture = loadTexture(test->openglContext, "res/container2_specular.png");

    prTextureData* steelTexture = loadTexture(test->openglContext, "res/steel.jpg");

    prTextureData* brickWallDiffuseTexture = loadTexture(test->openglContext, "res/brickWallDiffuse.tga");

    prTextureData* brickWallNormalTexture = loadTexture(test->openglContext, "res/brickWallNormal.tga");

    prTextureData* brickWallSpecularTexture = loadTexture(test->openglContext, "res/brickWallSpecular.tga");

    prTextureData* blackTexture = makeTextureSingleColor(test->openglContext, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});

    prTextureData* whiteTexture = makeTextureSingleColor(test->openglContext, (float[4]){1.0f, 1.0f, 1.0f, 1.0f});

    prMaterialData* materialMetal = prMaterialCreate();
    prMaterialLinkAmbientMap(materialMetal, defaultTexture);
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

    prMaterialData* materialBrick = prMaterialCreate();
    prMaterialLinkAmbientMap(materialBrick, brickWallDiffuseTexture);
    prMaterialLinkDiffuseMap(materialBrick, brickWallDiffuseTexture);
    prMaterialLinkSpecularMap(materialBrick, brickWallSpecularTexture);
    prMaterialLinkNormalMap(materialBrick, brickWallNormalTexture);
    prMaterialSetShininess(materialBrick, 32.0f);

    prMeshData* meshMetal = prMeshCreate();
    prMeshLinkContext(meshMetal, test->openglContext);
    prMeshUpdate(meshMetal,
        vertices, sizeof(vertices) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float));
    prMeshLinkMaterial(meshMetal, materialMetal);

    prMeshData* meshWood = prMeshCreate();
    prMeshLinkContext(meshWood, test->openglContext);
    prMeshUpdate(meshWood,
        vertices, sizeof(vertices) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float));
    prMeshLinkMaterial(meshWood, materialWood);

    prMeshData* meshWoodMetal = prMeshCreate();
    prMeshLinkContext(meshWoodMetal, test->openglContext);
    prMeshUpdate(meshWoodMetal,
        vertices, sizeof(vertices) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float));
    prMeshLinkMaterial(meshWoodMetal, materialWoodMetal);

    prMeshData* meshBrick = prMeshCreate();
    prMeshLinkContext(meshBrick, test->openglContext);
    prMeshUpdate(meshBrick,
        vertices, sizeof(vertices) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float));
    prMeshLinkMaterial(meshBrick, materialBrick);

    test->openglContext->UseProgram(shaderProgram->shaderProgramObject);
    prDirectionalLightData* sun = prDirectionalLightCreate();
    prDirectionalLightSetDirection(sun, (vec3){-1.0f, -1.0f, -1.0f});
    prDirectionalLightSetAmbient(sun, (vec3){0.2, 0.2, 0.15});
    prDirectionalLightSetDiffuse(sun, (vec3){0.5f, 0.5f, 0.4f});
    prDirectionalLightSetSpecular(sun, (vec3){0.7, 0.7, 0.7});

    prPointLightData* point = prPointLightCreate();
    point->constant = 1.0;
    point->linear = 0.7f;
    point->quadratic = 1.8f;
    prPointLightSetPosition(point, (vec3){0.0f, 0.0f, 0.0f});
    prPointLightSetDiffuse(point, (vec3){0.6f, 0.6f, 1.8f});

    prShaderProgramUniform3f(shaderProgram, "directionalLights[0].direction", sun->direction[0], sun->direction[1], sun->direction[2]);
    prShaderProgramUniform3f(shaderProgram, "directionalLights[0].ambient", sun->ambient[0], sun->ambient[1], sun->ambient[2]);
    prShaderProgramUniform3f(shaderProgram, "directionalLights[0].diffuse", sun->diffuse[0], sun->diffuse[1], sun->diffuse[2]);
    prShaderProgramUniform3f(shaderProgram, "directionalLights[0].specular", sun->specular[0], sun->specular[1], sun->specular[2]);

    prShaderProgramUniform1f(shaderProgram, "pointLights[0].constant", point->constant);
    prShaderProgramUniform1f(shaderProgram, "pointLights[0].linear", point->linear);
    prShaderProgramUniform1f(shaderProgram, "pointLights[0].quadratic", point->quadratic);

    prShaderProgramUniform3f(shaderProgram, "pointLights[0].position", point->position[0], point->position[1], point->position[2]);
    prShaderProgramUniform3f(shaderProgram, "pointLights[0].ambient", point->ambient[0], point->ambient[1], point->ambient[2]);
    prShaderProgramUniform3f(shaderProgram, "pointLights[0].diffuse", point->diffuse[0], point->diffuse[1], point->diffuse[2]);
    prShaderProgramUniform3f(shaderProgram, "pointLights[0].specular", point->specular[0], point->specular[1], point->specular[2]);

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

        vec3 rotation = {radians(yaw), radians(pitch), radians(0.0f)};
        prCameraUpdate(camera, cameraPosition, rotation, 45.0f, 0.001f, 10000.0f);

        translationsToMatrix(translation, (vec3){0.0f, 0.0f, -3.0f}, GLM_VEC3_ZERO, (vec3){10.0f, 10.0f, 0.5f});
        prMeshDraw(meshMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){1.0f, 0.0f, 0.0f}, (vec3){0.0f, sin(radians((glfwGetTimerValue() * 5.0f) / glfwGetTimerFrequency())), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshWood, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){-1.0f, 0.0f, 0.0f}, (vec3){0.0f, cos(radians((glfwGetTimerValue() * 5.0f) / glfwGetTimerFrequency())), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshWoodMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){0.0f, 1.5f, 0.0f}, (vec3){0.0f, tan(radians((glfwGetTimerValue() * 5.0f) / glfwGetTimerFrequency())), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshBrick, translation, camera, shaderProgram);

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

    prShaderProgramDestroy(shaderProgram);
    shaderProgram = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    prEndFileLogging();
}
