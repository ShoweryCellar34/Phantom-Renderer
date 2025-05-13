#include <PR/PR.h>
#include <PR/helperStuffs.h>

#include <stdio.h>
#include <cglm/cglm.h>
#include <PR/memory.h>
#include "exampleGlobalValues.h"
#include "exampleFunctions.h"

int main(int argc, char** argv) {
    prLogSetLevel(PR_LOG_TRACE);
    FILE* logFile = fopen("prLogLite.txt", "w");
    prLogSetStream(2, (FILE*[]){stdout, logFile});

    glfwInit();

    prWindow* test = prWindowCreate(TITLE, windowWidth, windowHeight);
    prWindowInitContext(test);

    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);
    glfwSetCursorPosCallback(test->window, cursorPosCallback);
    glfwSetKeyCallback(test->window, keyCallback);
    glfwSetInputMode(test->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(test->window, test->openglContext);

    prEnableImageFlip();

    prShaderData* shaderProgram = createDefaultShader(test->openglContext);

    prTextureData* diffuseTexture = makeTextureCheckerboard(test->openglContext, 8, (float[4]){1.0f, 0.0f, 1.0f, 1.0f}, (float[4]){0.0f, 0.0f, 0.0f, 1.0f});

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

    prMeshData* meshItem = prMeshCreate();
    prMeshLinkContext(meshItem, test->openglContext);
    prMeshUpdate(meshItem,
        vertices, sizeof(vertices) / sizeof(float),
        normals, sizeof(normals) / sizeof(float),
        textureCoordinates, sizeof(textureCoordinates) / sizeof(float),
        indices, sizeof(indices) / sizeof(unsigned int));

    // Set up lighting uniforms directly
    vec3 lightDir = {-0.25f, -0.5f, -0.75f};
    vec3 lightAmbient = {0.095f, 0.095f, 0.1f};
    vec3 lightDiffuse = {0.8f, 0.8f, 0.75f};
    vec3 lightSpecular = {0.9f, 0.9f, 0.85f};
    
    vec3 pointLightPos = {0.0f, 0.0f, 0.0f};
    vec3 pointLightDiffuse = {0.0f, 0.0f, 1.0f};
    vec3 pointLightSpecular = {1.0f, 0.0f, 0.0f};    // Set directional light uniforms
    prShaderSetUniform3f(shaderProgram, "directionalLights[0].direction", lightDir[0], lightDir[1], lightDir[2]);
    prShaderSetUniform3f(shaderProgram, "directionalLights[0].ambient", lightAmbient[0], lightAmbient[1], lightAmbient[2]);
    prShaderSetUniform3f(shaderProgram, "directionalLights[0].diffuse", lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
    prShaderSetUniform3f(shaderProgram, "directionalLights[0].specular", lightSpecular[0], lightSpecular[1], lightSpecular[2]);

    // Set point light uniforms
    prShaderSetUniform1f(shaderProgram, "pointLights[0].constant", 1.0f);
    prShaderSetUniform1f(shaderProgram, "pointLights[0].linear", 0.09f);
    prShaderSetUniform1f(shaderProgram, "pointLights[0].quadratic", 0.032f);

    prShaderSetUniform3f(shaderProgram, "pointLights[0].position", pointLightPos[0], pointLightPos[1], pointLightPos[2]);
    prShaderSetUniform3f(shaderProgram, "pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
    prShaderSetUniform3f(shaderProgram, "pointLights[0].diffuse", pointLightDiffuse[0], pointLightDiffuse[1], pointLightDiffuse[2]);
    prShaderSetUniform3f(shaderProgram, "pointLights[0].specular", pointLightSpecular[0], pointLightSpecular[1], pointLightSpecular[2]);

    camera = prCameraCreate();
    prCameraLinkContext(camera, test->openglContext);

    test->openglContext->Enable(GL_DEPTH_TEST);
    test->openglContext->ClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    test->openglContext->Enable(GL_CULL_FACE);
    test->openglContext->Enable(GL_BLEND);

    while(!glfwWindowShouldClose(test->window)) {
        test->openglContext->Enable(GL_DEPTH_TEST);
        prFramebufferClear(test->openglContext, NULL, PR_BUFFER_BIT_COLOR | PR_BUFFER_BIT_DEPTH);
        prFramebufferClear(test->openglContext, framebuffer, PR_BUFFER_BIT_COLOR | PR_BUFFER_BIT_DEPTH);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        vec3 rotation = {radians(yaw), radians(pitch), radians(0.0f)};
        prCameraUpdate(camera, cameraPosition, rotation, 45.0f, 0.1f, 100.0f);
        mat4 translation;
        translationsToMatrix(translation, GLM_VEC3_ZERO, GLM_VEC3_ZERO, GLM_VEC3_ONE);

        prShaderSetUniform3f(shaderProgram, "cameraPosition", camera->position[0], camera->position[1], camera->position[2]);
        prShaderSetUniformMatrix4fv(shaderProgram, "view", camera->view[0]);
        prShaderSetUniformMatrix4fv(shaderProgram, "projection", camera->projection[0]);
        prShaderSetUniformMatrix4fv(shaderProgram, "translation", translation[0]);

        prMeshDraw(meshItem);

        glfwSwapBuffers(test->window);
        glfwPollEvents();
        proccessInput(test->window);
    }

    prCameraDestroy(camera);
    camera = NULL;

    prFramebufferDestroy(framebuffer);
    framebuffer = NULL;
    prRenderBufferDestroy(depthStencilRBO);
    depthStencilRBO = NULL;
    prTextureDestroy(colorTexture);
    colorTexture = NULL;

    prTextureDestroy(diffuseTexture);
    diffuseTexture = NULL;

    prMeshDestroy(meshItem);
    meshItem = NULL;

    prShaderDestroy(shaderProgram);
    shaderProgram = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    fclose(logFile);
    logFile = NULL;
}
