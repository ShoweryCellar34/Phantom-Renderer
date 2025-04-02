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
    prLogSetStream(0, (FILE*[]){stdout, logFile});

    glfwInit();

    prWindow* test = prWindowCreate("Test", 1280, 720);
    prWindowInitContext(test);
    glfwSetWindowUserPointer(test->window, test->openglContext);

    prEnableImageFlip();

    prShaderProgramData* shaderProgram = loadDefaultShader(test->openglContext);

    prShaderProgramData* shaderProgram2 = prShaderProgramCreate();
    prShaderProgramLinkContext(shaderProgram2, test->openglContext);
    prShaderProgramUpdate(shaderProgram2, BASE_VERTEX_SHADER, OUTLINE_FRAGMENT_SHADER);

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
    prPointLightSetDiffuse(point, (vec3){0.6f, 0.6f, 1.0f});

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

    camera = prCameraCreate();
    prCameraLinkContext(camera, test->openglContext);

    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);
    glfwSetCursorPosCallback(test->window, cursorPosCallback);
    glfwSetInputMode(test->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    test->openglContext->StencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    test->openglContext->Enable(GL_CULL_FACE);
    test->openglContext->Enable(GL_BLEND);
    test->openglContext->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    while(!glfwWindowShouldClose(test->window)) {
        test->openglContext->Enable(GL_DEPTH_TEST);
        test->openglContext->Enable(GL_STENCIL_TEST);
        test->openglContext->StencilFunc(GL_ALWAYS, 1, 0xFF);
        test->openglContext->StencilMask(0xFF);
        test->openglContext->ClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        test->openglContext->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        mat4 translation;

        vec3 rotation = {radians(yaw), radians(pitch), radians(0.0f)};
        prCameraUpdate(camera, cameraPosition, rotation, 45.0f, 0.1f, 100.0f);

        float val = smoothOvertimeSin();

        translationsToMatrix(translation, (vec3){0.0f, 0.0f, -20.0f}, GLM_VEC3_ZERO, (vec3){30.0f, 30.0f, 10.0f});
        prMeshDraw(meshMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){1.0f, 0.0f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshWood, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){-1.0f, 0.0f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshWoodMetal, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){0.0f, 1.5f, 0.0f}, (vec3){0.0f, val, 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshBrick, translation, camera, shaderProgram);

        translationsToMatrix(translation, (vec3){0.0f, val / 3.5f - 1.5f, 0.0f}, (vec3){0.0f, radians(val * 100.0f), 0.0f}, GLM_VEC3_ONE);
        prMeshDraw(meshItem, translation, camera, shaderProgram);

        test->openglContext->StencilFunc(GL_NOTEQUAL, 1, 0xFF);
        test->openglContext->StencilMask(0x00);
        test->openglContext->Disable(GL_DEPTH_TEST);

        translationsToMatrix(translation, (vec3){0.0f, 0.0f, -20.0f}, GLM_VEC3_ZERO, (vec3){33.0f, 33.0f, 11.0f});
        prMeshDraw(meshMetal, translation, camera, shaderProgram2);

        translationsToMatrix(translation, (vec3){1.0f, 0.0f, 0.0f}, (vec3){0.0f, val, 0.0f}, (vec3){1.1f, 1.1f, 1.1f});
        prMeshDraw(meshWood, translation, camera, shaderProgram2);

        translationsToMatrix(translation, (vec3){-1.0f, 0.0f, 0.0f}, (vec3){0.0f, val, 0.0f}, (vec3){1.1f, 1.1f, 1.1f});
        prMeshDraw(meshWoodMetal, translation, camera, shaderProgram2);

        translationsToMatrix(translation, (vec3){0.0f, 1.5f, 0.0f}, (vec3){0.0f, val, 0.0f}, (vec3){1.1f, 1.1f, 1.1f});
        prMeshDraw(meshBrick, translation, camera, shaderProgram2);

        translationsToMatrix(translation, (vec3){0.0f, val / 3.5f - 1.5f, 0.0f}, (vec3){0.0f, radians(val * 100.0f), 0.0f}, (vec3){1.1f, 1.1f, 1.1f});
        prMeshDraw(meshItem, translation, camera, shaderProgram2);

        glfwSwapBuffers(test->window);
        glfwPollEvents();
        proccessInput(test->window);
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
    prMaterialDestroy(materialWhite);
    materialWhite = NULL;
    prMaterialDestroy(defaultMaterial);
    defaultMaterial = NULL;

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

    prShaderProgramDestroy(shaderProgram2);
    shaderProgram2 = NULL;
    prShaderProgramDestroy(shaderProgram);
    shaderProgram = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    fclose(logFile);
    logFile = NULL;
}
