#include <globals.hpp>

#include <PR/PR.h>
#include <setup.hpp>

void translationsToMatrix(mat4 matrix, vec3 position, vec3 rotation, vec3 scale) {
    glm_mat4_identity(matrix);
    glm_translate(matrix, position);
    mat3 temp = {{0.0f, 0.0f, 1.0f},
                 {0.0f, 1.0f, 0.0f},
                 {1.0f, 0.0f, 0.0f}};
    glm_rotate(matrix, rotation[2], temp[0]);
    glm_rotate(matrix, rotation[1], temp[1]);
    glm_rotate(matrix, rotation[0], temp[2]);
    glm_scale(matrix, scale);
}

void proccessInput(GLFWwindow* window) {
    mat4x3 temp;
    mat2x3 temp2;
    glm_mat4x3_zero(temp);

    float cameraSpeed = 7.5f * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraSpeed *= 2.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        cameraSpeed /= 2.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_scale(camera->front, cameraSpeed, temp[0]);
        glm_vec3_add(cameraPosition, temp[0], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_scale(camera->front, cameraSpeed, temp[1]);
        glm_vec3_sub(cameraPosition, temp[1], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_cross(camera->front, camera->up, temp[2]);
        glm_normalize(temp[2]);
        glm_vec3_scale(temp[2], cameraSpeed, temp[2]);
        glm_vec3_sub(cameraPosition, temp[2], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_cross(camera->front, camera->up, temp[3]);
        glm_normalize(temp[3]);
        glm_vec3_scale(temp[3], cameraSpeed, temp[3]);
        glm_vec3_add(cameraPosition, temp[3], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        glm_vec3_scale(camera->up, cameraSpeed, temp[1]);
        glm_vec3_add(cameraPosition, temp[1], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        glm_vec3_scale(camera->up, cameraSpeed, temp[1]);
        glm_vec3_sub(cameraPosition, temp[1], cameraPosition);
    }

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glm_cross(camera->up, camera->front, temp2[0]);
        glm_normalize(temp2[0]);
        glm_cross(camera->front, temp2[0], temp2[0]);
        glm_vec3_scale(temp2[0], cameraSpeed, temp2[0]);
        glm_vec3_add(cameraPosition, temp2[0], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        glm_cross(camera->up, camera->front, temp2[0]);
        glm_normalize(temp2[0]);
        glm_cross(camera->front, temp2[0], temp2[0]);
        glm_vec3_scale(temp2[0], cameraSpeed, temp2[0]);
        glm_vec3_sub(cameraPosition, temp2[0], cameraPosition);
    }
}

int main(int argc, char** argv) {
    setupPaths();
    setLogFilePath(g_userDataPath / "prLog.txt");
    setupLog();

    setupWindow();
    setupShaders();
    setupFramebuffers();
    setupTextures();
    setupMaterials();
    setupMeshes();

    typedef struct directionalLightData {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        GLuint shadowMap;
    } directionalLightData;

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

    directionalLightData sun = {
        {-0.25f, -0.5f, -0.75f},
        {0.02f, 0.015f, 0.015f},
        {0.6f, 0.6f, 0.55f},
        {1.3f, 1.3f, 1.25f},
        4
    };

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
    prCameraLinkContext(camera, g_window->openglContext);

    mat4 lightProjection;
    glm_ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 100.0f, lightProjection);
    mat4 lightView;
    glm_lookat((vec3){40.0f, 40.0f, 40.0f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, lightView);
    mat4 lightSpaceMatrix;
    glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);
    prShaderSetUniformMatrix4fv(g_shaderDirectionalLight, "lightSpaceMatrix", lightSpaceMatrix[0]);

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
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[0]", light2SpaceMatrix[0][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[1]", light2SpaceMatrix[1][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[2]", light2SpaceMatrix[2][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[3]", light2SpaceMatrix[3][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[4]", light2SpaceMatrix[4][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[5]", light2SpaceMatrix[5][0]);
    prShaderSetUniform3f(g_shaderPointLight, "lightPosition", point.position[0], point.position[1], point.position[2]);
    prShaderSetUniform1f(g_shaderPointLight, "farPlane", 100.0f);

    g_window->openglContext->Enable(GL_DEPTH_TEST);
    g_window->openglContext->Enable(GL_BLEND);

    glfwMaximizeWindow(g_window->window);

    prShaderSetUniform2f(g_shaderDebug, "screenSize", g_windowWidth, g_windowHeight);

    while(!glfwWindowShouldClose(g_window->window)) {
        g_window->openglContext->Enable(GL_DEPTH_TEST);
        prFramebufferClearColor(g_window->openglContext, NULL, 0, TEMP_RGBA(0.3f, 0.5f, 0.7f, 1.0f));
        prFramebufferClearDepth(g_window->openglContext, NULL, 1.0f);
        prFramebufferClearDepth(g_window->openglContext, g_framebufferSunShadowMap, 1.0f);
        prFramebufferClearDepth(g_window->openglContext, g_framebufferPointShadowMap, 1.0f);
        prFramebufferClearColor(g_window->openglContext, g_framebufferGaussianBlur1, 0, TEMP_RGBA(0.0f, 0.0f, 0.0f, 0.0f));
        prFramebufferClearColor(g_window->openglContext, g_framebufferGaussianBlur2, 0, TEMP_RGBA(0.0f, 0.0f, 0.0f, 0.0f));
        prFramebufferClearColor(g_window->openglContext, g_framebufferBloom, 0, TEMP_RGBA(0.0f, 0.0f, 0.0f, 0.0f));
        prFramebufferClearColor(g_window->openglContext, g_framebufferMultisampled, 0, TEMP_RGBA(0.7f, 0.5f, 0.3f, 1.0f));
        prFramebufferClearColor(g_window->openglContext, g_framebufferMultisampled, 1, TEMP_RGBA(0.7f, 0.5f, 0.3f, 1.0f));
        prFramebufferClearDepthStencil(g_window->openglContext, g_framebufferMultisampled, 1.0f, 0);
        prFramebufferClearColor(g_window->openglContext, g_framebufferDefault, 0, TEMP_RGBA(0.3f, 0.5f, 0.7f, 1.0f));
        prFramebufferClearDepthStencil(g_window->openglContext, g_framebufferDefault, 1.0f, 0);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        mat4 translation;

        vec3 rotation = {glm_rad(yaw), glm_rad(pitch), glm_rad(0.0f)};
        prCameraUpdate(camera, cameraPosition, rotation, 45.0f, 0.1f, 1500.0f);

        prShaderData* currentShaderProgram = (useDebugShader ? g_shaderDebug : g_shaderDefault);

        static float seed = 0;
        seed += deltaTime;
        float smoothSinOverTime = sin(seed);
        float smoothOverTime = seed;

        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].direction", sun.direction[0], sun.direction[1], sun.direction[2]);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].ambient", sun.ambient[0], sun.ambient[1], sun.ambient[2]);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].diffuse", sun.diffuse[0], sun.diffuse[1], sun.diffuse[2]);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].specular", sun.specular[0], sun.specular[1], sun.specular[2]);
        prTextureBindTexture(g_textureDepthSunShadowMap, sun.shadowMap);
        prShaderSetUniform1i(currentShaderProgram, "directionalLights[0].shadowMap", sun.shadowMap);

        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].constant", point.constant);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].linear", point.linear);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].quadratic", point.quadratic);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].position", point.position[0], point.position[1], point.position[2]);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].ambient", point.ambient[0], point.ambient[1], point.ambient[2]);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].diffuse", point.diffuse[0], point.diffuse[1], point.diffuse[2]);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].specular", point.specular[0], point.specular[1], point.specular[2]);
        prCubeMapBindTexture(g_cubeMapDepthPointShadowMap, point.shadowMap);
        prShaderSetUniform1i(currentShaderProgram, "pointLights[0].shadowMap", point.shadowMap);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].farPlane", point.farPlane);

        prShaderSetUniform3f(currentShaderProgram, "cameraPosition", camera->position[0], camera->position[1], camera->position[2]);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "view", camera->view[0]);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "projection", camera->projection[0]);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "lightSpaceMatrix", lightSpaceMatrix[0]);

        for(int i = 0; i < 3; i++) {
            switch(i) {
                case 0:
                    prFramebufferBind(g_framebufferSunShadowMap);
                    currentShaderProgram = g_shaderDirectionalLight;
                    g_window->openglContext->Viewport(0, 0, 2048, 2048);
                    break;

                case 1:
                    prFramebufferBind(g_framebufferPointShadowMap);
                    currentShaderProgram = g_shaderPointLight;
                    g_window->openglContext->Viewport(0, 0, 1024, 1024);
                    break;

                case 2:
                    prFramebufferBind(g_framebufferMultisampled);
                    currentShaderProgram = (useDebugShader ? g_shaderDebug : g_shaderDefault);
                    g_window->openglContext->Viewport(0, 0, g_windowWidth, g_windowHeight);
                    break;
            }

            translationsToMatrix(translation, (vec3){0.0f, 0.0f, -30.0f}, VEC3_ZERO, (vec3){30.0f, 30.0f, 30.0f});
            g_materialContainer.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);

            translationsToMatrix(translation, (vec3){0.0f, -30.0f, 0.0f}, VEC3_ZERO, (vec3){30.0f, 30.0f, 30.0f});
            g_materialSteel.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);

            translationsToMatrix(translation, (vec3){-30.0f, 0.0f, 0.0f}, VEC3_ZERO, (vec3){30.0f, 30.0f, 30.0f});
            g_materialBrickWall.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);

            translationsToMatrix(translation, (vec3){2.0f, 0.0f, 0.0f}, (vec3){0.0f, smoothSinOverTime, 0.0f}, VEC3_ONE);
            g_materialBlack.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);

            translationsToMatrix(translation, (vec3){-2.0f, 0.0f, 0.0f}, (vec3){0.0f, smoothSinOverTime, 0.0f}, VEC3_ONE);
            g_materialMetalRimmedContainer.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);

            translationsToMatrix(translation, (vec3){0.0f, 2.0f, 0.0f}, (vec3){0.0f, smoothOverTime, 0.0f}, VEC3_ONE);
            g_materialBrickWall.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);

            translationsToMatrix(translation, (vec3){0.0f, -2.0f, 0.0f}, (vec3){0.0f, glm_rad(smoothOverTime * 100.0f), 0.0f}, VEC3_ONE);
            g_materialWhite.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);

            translationsToMatrix(translation, (vec3){0.0f, 0.0f, 2.0f}, (vec3){0.0f, glm_rad(smoothOverTime * 100.0f), 0.0f}, VEC3_ONE);
            g_materialSteel.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);

            translationsToMatrix(translation, (vec3){0.0f, 0.0f, -2.0f}, (vec3){0.0f, glm_rad(smoothSinOverTime * 100.0f), 0.0f}, VEC3_ONE);
            g_materialSteel.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", translation[0]);
            prMeshDrawIndices(g_meshCube);
        }

        switch(currentSkybox) {
            case 1:
                prCubeMapBindTexture(g_cubeMapIslands, 0);
                break;

            case 2:
                prCubeMapBindTexture(g_cubeMapSpace, 0);
                break;

            case 3:
                prCubeMapBindTexture(g_cubeMap3, 0);
                break;

            case 4:
                prCubeMapBindTexture(g_cubeMap4, 0);
                break;

            default:
                prCubeMapBindTexture(skyboxDefaultCubeMap, 0);
                break;
        }

        translationsToMatrix(translation, camera->position, VEC3_ZERO, VEC3_ONE);
        prShaderSetUniformMatrix4fv(g_shaderSkybox, "translation", translation[0]);
        prShaderSetUniformMatrix4fv(g_shaderSkybox, "view", camera->view[0]);
        prShaderSetUniformMatrix4fv(g_shaderSkybox, "projection", camera->projection[0]);
        g_window->openglContext->DepthFunc(GL_LEQUAL);
        prShaderBind(g_shaderSkybox);
        prMeshDrawIndices(g_meshCube);

        if(showHUD == 1) {
            g_window->openglContext->Disable(GL_DEPTH_TEST);
            g_materialHUD.bind(g_shaderHUD);
            prMeshDrawIndices(g_meshQuad);
        }

        prFramebufferBlit(g_window->openglContext, g_framebufferMultisampled, g_framebufferDefault,
            0, 0, g_windowWidth, g_windowHeight,
            0, 0, g_windowWidth, g_windowHeight,
            PR_BUFFER_BIT_COLOR, PR_FILTER_NEAREST
        );
        prFramebufferBind(g_framebufferDefault);

        if(showPostProcessing) {
            g_window->openglContext->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            prTextureBindTexture(g_textureColorDefault, 0);
            prTextureBindImage(g_texturePostProcessing, 1, 0, PR_ACCESS_WRITE_ONLY, GL_RGBA32F);
            prComputeShaderDispatch(g_computeShaderPostProcessing, g_windowWidth , g_windowHeight, 1);
            g_window->openglContext->MemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

            g_materialPostProcessing.bind(g_shaderHUD);
            prMeshDrawIndices(g_meshQuad);
        }

        prFramebufferSetReadBuffer(g_framebufferMultisampled, PR_COLOR_ATTACHMENT_1);
        prFramebufferBlit(g_window->openglContext, g_framebufferMultisampled, g_framebufferBloom,
            0, 0, g_windowWidth, g_windowHeight,
            0, 0, g_windowWidth, g_windowHeight,
            PR_BUFFER_BIT_COLOR, PR_FILTER_NEAREST
        );
        prFramebufferSetReadBuffer(g_framebufferMultisampled, PR_COLOR_ATTACHMENT_0);
        bool horizontal = true, firstIteration = true;
        int amount = BLOOM_BLUR_PASSES;
        prShaderBind(g_shaderGaussianBlur);
        prShaderSetUniform1i(g_shaderGaussianBlur, "image", 0);
        for(int i = 0; i < amount; i++) {
            prFramebufferBind(g_gaussianBlurFramebuffers[horizontal]);
            prShaderSetUniform1i(g_shaderGaussianBlur, "horizontal", horizontal);
            prTextureBindTexture(firstIteration == true ? g_textureBloom : g_gaussianBlurTextures[!horizontal], 0);
            g_window->openglContext->Disable(GL_DEPTH_TEST);
            prMeshDrawIndices(g_meshQuad);
            horizontal = !horizontal;
            if(firstIteration) {
                firstIteration = false;
            }
        }
        prFramebufferBind(g_framebufferDefault);

        g_window->openglContext->Disable(GL_DEPTH_TEST);
        prTextureBindTexture(g_textureColorDefault, 0);
        prTextureBindTexture(g_gaussianBlurTextures[!horizontal], 1);
        prShaderSetUniform1i(g_shaderHDR, "scene", 0);
        prShaderSetUniform1i(g_shaderHDR, "bloomBlur", 1);
        prShaderSetUniform1f(g_shaderHDR, "exposure", 1.2f);
        prShaderBind(g_shaderHDR);
        prMeshDrawIndices(g_meshQuad);

        prFramebufferBlit(g_window->openglContext, g_framebufferDefault, NULL,
            0, 0, g_windowWidth, g_windowHeight,
            0, 0, g_windowWidth, g_windowHeight,
            PR_BUFFER_BIT_COLOR, PR_FILTER_NEAREST
        );

        glfwSwapBuffers(g_window->window);
        glfwPollEvents();
        proccessInput(g_window->window);
    }

    prCameraDestroy(camera);
    camera = NULL;

    shutdownMeshes();
    shutdownMaterials();
    shutdownTextures();
    shutdownFramebuffers();
    shutdownShaders();
    shutdownWindow();

    shutdownLog();
    return 0;
}
