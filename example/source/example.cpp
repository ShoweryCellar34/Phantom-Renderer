#include <globals.hpp>

#include <PR/PR.h>
#include <setup.hpp>

mat4s translationsToMatrix(vec3s position, vec3s rotation, vec3s scale) {
    mat4s matrix = glms_mat4_identity();
    matrix = glms_translate(matrix, position);
    float tempData[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    mat3s temp = glms_mat3_make(tempData);
    matrix = glms_rotate(matrix, rotation.z, temp.col[0]);
    matrix = glms_rotate(matrix, rotation.y, temp.col[1]);
    matrix = glms_rotate(matrix, rotation.x, temp.col[2]);
    matrix = glms_scale(matrix, scale);

    return matrix;
}

void proccessInput(GLFWwindow* window) {
    float cameraSpeed = 7.5f * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraSpeed *= 2.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        cameraSpeed /= 2.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition = glms_vec3_add(cameraPosition, glms_vec3_scale(g_camera->front, cameraSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition = glms_vec3_sub(cameraPosition, glms_vec3_scale(g_camera->front, cameraSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition = glms_vec3_sub(cameraPosition, glms_vec3_scale(glms_normalize(glms_cross(g_camera->front, g_camera->up)), cameraSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition = glms_vec3_add(cameraPosition, glms_vec3_scale(glms_normalize(glms_cross(g_camera->front, g_camera->up)), cameraSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPosition = glms_vec3_add(cameraPosition, glms_vec3_scale(g_camera->up, cameraSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraPosition = glms_vec3_sub(cameraPosition, glms_vec3_scale(g_camera->up, cameraSpeed));
    }

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraPosition = glms_vec3_add(cameraPosition, glms_vec3_scale(glms_cross(g_camera->front, glms_normalize(glms_cross(g_camera->up, g_camera->front))), cameraSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cameraPosition = glms_vec3_sub(cameraPosition, glms_vec3_scale(glms_cross(g_camera->front, glms_normalize(glms_cross(g_camera->up, g_camera->front))), cameraSpeed));
    }
}

int main(int argc, char** argv) {
    setupPaths();
    setLogFilePath(TO_USR("logs/prLog.txt"));
    setupLog();

    setupWindow();
    setupShaders();
    setupFramebuffers();
    setupTextures();
    setupMaterials();
    setupMeshes();

    typedef struct directionalLightData {
        vec3s direction;
        vec3s ambient;
        vec3s diffuse;
        vec3s specular;

        float nearPlane;
        float farPlane;
        int width;
        int height;

        GLuint shadowMap;
    } directionalLightData;

    typedef struct pointLightData {
        float constant;
        float linear;
        float quadratic;

        vec3s position;

        vec3s ambient;
        vec3s diffuse;
        vec3s specular;

        float nearPlane;
        float farPlane;
        int resolution;

        GLuint shadowMap;
    } pointLightData;

    directionalLightData sun = {
        {-0.25f, -0.5f, -0.75f},
        {0.02f, 0.015f, 0.015f},
        {0.6f, 0.6f, 0.55f},
        {1.0f, 1.0f, 0.95f},
        5.0f,
        200.0f,
        SUN_LIGHT_WIDTH,
        SUN_LIGHT_HEIGHT,
        4
    };

    pointLightData point = {
        1.0f,
        0.22f,
        0.2f,
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f},
        1.0f,
        100.0f,
        POINT_LIGHT_RESOLUTION,
        5
    };

    g_camera = prCameraCreate();
    prCameraLinkContext(g_camera, g_window->openglContext);
    vec3s rotation = {glm_rad(yaw), glm_rad(pitch), glm_rad(0.0f)};
    prCameraUpdate(g_camera, cameraPosition, rotation, 45.0f, 0.1f, 1500.0f);

    float aspectRatio = sun.width / sun.height;
    mat4s lightProjection = glms_ortho(-50.0f * aspectRatio, 50.0f, -50.0f, 50.0f, sun.nearPlane, sun.farPlane);
    mat4s lightView = glms_lookat(glms_vec3_scale(glms_vec3_negate(sun.direction), 40.0f), {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    mat4s lightSpaceMatrix = glms_mat4_mul(lightProjection, lightView);
    prShaderSetUniformMatrix4fv(g_shaderDirectionalLight, "lightSpaceMatrix", &lightSpaceMatrix.raw[0][0]);

    float aspectRatio2 = (float)point.resolution / (float)point.resolution;
    mat4s light2Projection = glms_perspective(glm_rad(90.0f), aspectRatio2, point.nearPlane, point.farPlane);
    mat4s light2View[6] = {
        glms_lookat(point.position, {point.position.x + 1.0f, point.position.y, point.position.z}, {0.0f, -1.0f, 0.0f}),
        glms_lookat(point.position, {point.position.x + -1.0f, point.position.y, point.position.z}, {0.0f, -1.0f, 0.0f}),
        glms_lookat(point.position, {point.position.x, point.position.y + 1.0f, point.position.z}, {0.0f, 0.0f, 1.0f}),
        glms_lookat(point.position, {point.position.x, point.position.y + -1.0f, point.position.z}, {0.0f, 0.0f, -1.0f}),
        glms_lookat(point.position, {point.position.x, point.position.y, point.position.z + 1.0f}, {0.0f, -1.0f, 0.0f}),
        glms_lookat(point.position, {point.position.x, point.position.y, point.position.z + -1.0f}, {0.0f, -1.0f, 0.0f})
    };
    mat4s light2SpaceMatrix[6] = {
        glms_mat4_mul(light2Projection, light2View[0]),
        glms_mat4_mul(light2Projection, light2View[1]),
        glms_mat4_mul(light2Projection, light2View[2]),
        glms_mat4_mul(light2Projection, light2View[3]),
        glms_mat4_mul(light2Projection, light2View[4]),
        glms_mat4_mul(light2Projection, light2View[5])
    };
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[0]", &light2SpaceMatrix[0].raw[0][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[1]", &light2SpaceMatrix[1].raw[0][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[2]", &light2SpaceMatrix[2].raw[0][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[3]", &light2SpaceMatrix[3].raw[0][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[4]", &light2SpaceMatrix[4].raw[0][0]);
    prShaderSetUniformMatrix4fv(g_shaderPointLight, "lightSpaceMatrices[5]", &light2SpaceMatrix[5].raw[0][0]);
    prShaderSetUniform3f(g_shaderPointLight, "lightPosition", point.position.x, point.position.y, point.position.z);
    prShaderSetUniform1f(g_shaderPointLight, "farPlane", point.farPlane);

    g_window->openglContext->Enable(GL_DEPTH_TEST);
    g_window->openglContext->Enable(GL_CULL_FACE);
    g_window->openglContext->FrontFace(GL_CCW);
    g_window->openglContext->Enable(GL_BLEND);

    glfwMaximizeWindow(g_window->window);

    prShaderSetUniform2f(g_shaderDebug, "screenSize", g_windowWidth, g_windowHeight);

    while(!glfwWindowShouldClose(g_window->window)) {
        g_window->openglContext->Enable(GL_DEPTH_TEST);
        prFramebufferClearColor(g_window->openglContext, NULL, 0, {0.3f, 0.5f, 0.7f, 1.0f});
        prFramebufferClearDepth(g_window->openglContext, NULL, 1.0f);
        prFramebufferClearDepth(g_window->openglContext, g_framebufferSunShadowMap, 1.0f);
        prFramebufferClearDepth(g_window->openglContext, g_framebufferPointShadowMap, 1.0f);
        prFramebufferClearColor(g_window->openglContext, g_framebufferGaussianBlur1, 0, {0.0f, 0.0f, 0.0f, 0.0f});
        prFramebufferClearColor(g_window->openglContext, g_framebufferGaussianBlur2, 0, {0.0f, 0.0f, 0.0f, 0.0f});
        prFramebufferClearColor(g_window->openglContext, g_framebufferBloom, 0, {0.0f, 0.0f, 0.0f, 0.0f});
        prFramebufferClearColor(g_window->openglContext, g_framebufferMultisampled, 0, {0.7f, 0.5f, 0.3f, 1.0f});
        prFramebufferClearColor(g_window->openglContext, g_framebufferMultisampled, 1, {0.7f, 0.5f, 0.3f, 1.0f});
        prFramebufferClearDepthStencil(g_window->openglContext, g_framebufferMultisampled, 1.0f, 0);
        prFramebufferClearColor(g_window->openglContext, g_framebufferDefault, 0, {0.3f, 0.5f, 0.7f, 1.0f});
        prFramebufferClearDepthStencil(g_window->openglContext, g_framebufferDefault, 1.0f, 0);

        float currentFrame = glfwGetTime() / 1.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        vec3s rotation = {glm_rad(yaw), glm_rad(pitch), glm_rad(0.0f)};
        prCameraUpdate(g_camera, cameraPosition, rotation, 45.0f, 0.1f, 1500.0f);

        prShaderData* currentShaderProgram = (useDebugShader ? g_shaderDebug : g_shaderDefault);

        static float seed = 0;
        seed += deltaTime;
        float smoothSinOverTime = sin(seed);
        float smoothOverTime = seed;

        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].direction", sun.direction.x, sun.direction.y, sun.direction.z);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].ambient", sun.ambient.x, sun.ambient.y, sun.ambient.z);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].diffuse", sun.diffuse.x, sun.diffuse.y, sun.diffuse.z);
        prShaderSetUniform3f(currentShaderProgram, "directionalLights[0].specular", sun.specular.x, sun.specular.y, sun.specular.z);
        prTextureBindTexture(g_textureDepthSunShadowMap, sun.shadowMap);
        prShaderSetUniform1i(currentShaderProgram, "directionalLights[0].shadowMap", sun.shadowMap);

        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].constant", point.constant);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].linear", point.linear);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].quadratic", point.quadratic);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].position", point.position.x, point.position.y, point.position.z);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].ambient", point.ambient.x, point.ambient.y, point.ambient.z);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].diffuse", point.diffuse.x, point.diffuse.y, point.diffuse.z);
        prShaderSetUniform3f(currentShaderProgram, "pointLights[0].specular", point.specular.x, point.specular.y, point.specular.z);
        prCubeMapBindTexture(g_cubeMapDepthPointShadowMap, point.shadowMap);
        prShaderSetUniform1i(currentShaderProgram, "pointLights[0].shadowMap", point.shadowMap);
        prShaderSetUniform1f(currentShaderProgram, "pointLights[0].farPlane", point.farPlane);

        prShaderSetUniform3f(currentShaderProgram, "cameraPosition", g_camera->position.x, g_camera->position.y, g_camera->position.z);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "view", &g_camera->view.raw[0][0]);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "projection", &g_camera->projection.raw[0][0]);
        prShaderSetUniformMatrix4fv(currentShaderProgram, "lightSpaceMatrix", &lightSpaceMatrix.raw[0][0]);

        for(int i = 0; i < 3; i++) {
            switch(i) {
                case 0:
                    g_window->openglContext->CullFace(GL_FRONT);
                    prFramebufferBind(g_framebufferSunShadowMap);
                    currentShaderProgram = g_shaderDirectionalLight;
                    g_window->openglContext->Viewport(0, 0, sun.width, sun.height);
                    break;

                case 1:
                    g_window->openglContext->CullFace(GL_FRONT);
                    prFramebufferBind(g_framebufferPointShadowMap);
                    currentShaderProgram = g_shaderPointLight;
                    g_window->openglContext->Viewport(0, 0, point.resolution, point.resolution);
                    break;

                case 2:
                    g_window->openglContext->CullFace(GL_BACK);
                    prFramebufferBind(g_framebufferMultisampled);
                    currentShaderProgram = (useDebugShader ? g_shaderDebug : g_shaderDefault);
                    g_window->openglContext->Viewport(0, 0, g_windowWidth, g_windowHeight);
                    break;
            }

            g_materialContainer.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({0.0f, 0.0f, -30.0f}, {0.0f, 0.0f, 0.0f}, {30.1f, 30.1f, 30.1f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialSteel.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({0.0f, -30.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {30.1f, 30.1f, 30.1f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialBrickWall.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({-30.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {30.1f, 30.1f, 30.1f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialCheckerboard.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({-115.0f, -20.0f, -115.0f}, {0.0f, 0.0f, 0.0f}, {200.01f, 10.01f, 200.01f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialBrickWall.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({2.0f, 0.0f, 0.0f}, {0.0f, smoothSinOverTime, 0.0f}, {1.0f, 1.0f, 1.0f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialSteel.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({-2.0f, 0.0f, 0.0f}, {0.0f, smoothSinOverTime, 0.0f}, {1.0f, 1.0f, 1.0f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialWhite.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({0.0f, 2.0f, 0.0f}, {0.0f, smoothSinOverTime, 0.0f}, {1.0f, 1.0f, 1.0f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialBlack.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({0.0f, -2.0f, 0.0f}, {0.0f, glm_rad(smoothSinOverTime * 100.0f), 0.0f}, {1.0f, 1.0f, 1.0f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialMetalRimmedContainer.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({0.0f, 0.0f, 2.0f}, {0.0f, glm_rad(smoothSinOverTime * 100.0f), 0.0f}, {1.0f, 1.0f, 1.0f}).raw[0][0]);
            prMeshDrawIndices(g_meshCube);

            g_materialCheckerboard.bind(currentShaderProgram);
            prShaderSetUniformMatrix4fv(currentShaderProgram, "translation", &translationsToMatrix({0.0f, 0.0f, -2.0f}, {0.0f, glm_rad(smoothSinOverTime * 100.0f), 0.0f}, {1.0f, 1.0f, 1.0f}).raw[0][0]);
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
                prCubeMapBindTexture(g_cubeMapBlack, 0);
                break;
        }

        prShaderSetUniformMatrix4fv(g_shaderSkybox, "translation", &translationsToMatrix(g_camera->position, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}).raw[0][0]);
        prShaderSetUniformMatrix4fv(g_shaderSkybox, "view", &g_camera->view.raw[0][0]);
        prShaderSetUniformMatrix4fv(g_shaderSkybox, "projection", &g_camera->projection.raw[0][0]);
        prShaderSetUniform1i(g_shaderSkybox, "skybox", 0);
        g_window->openglContext->DepthFunc(GL_LEQUAL);
        prShaderBind(g_shaderSkybox);
        g_window->openglContext->FrontFace(GL_CW);
        prMeshDrawIndices(g_meshCube);
        g_window->openglContext->FrontFace(GL_CCW);

        if(showHUD == 1) {
            g_window->openglContext->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    screenShotThread->join();

    prCameraDestroy(g_camera);
    g_camera = NULL;

    shutdownMeshes();
    shutdownMaterials();
    shutdownTextures();
    shutdownFramebuffers();
    shutdownShaders();
    shutdownWindow();

    shutdownLog();
    return 0;
}
