#pragma once

#define M_PI 3.14159265358979323846
#include <math.h>
#include <time.h>
#include <string.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_MALLOC prMalloc
#define STBIW_REALLOC prRealloc
#define STBIW_FREE prFree
#include <stb_image_write.h>
#include "exampleGlobalValues.h"

void* computeGPUReadyBuffer(int* size,
    GLfloat* vertices, size_t verticesCount,
    GLfloat* normals, size_t normalsCount,
    GLfloat* textureCoordinates, size_t textureCoordinatesCount
) {
    *size = (verticesCount / 3) * 8 * sizeof(GLfloat);
    GLfloat* GPUReadyBuffer = prMalloc(*size);

    size_t index = 0;
    for(size_t i = 0; i < verticesCount / 3; i++) {
        GPUReadyBuffer[index++] = vertices[i * 3];
        GPUReadyBuffer[index++] = vertices[i * 3 + 1];
        GPUReadyBuffer[index++] = vertices[i * 3 + 2];
        // printf("Vertex %i:\n    XYZ:  %.3f, %.3f, %.3f\n", i, mesh->GPUReadyBuffer[index - 3], mesh->GPUReadyBuffer[index - 2], mesh->GPUReadyBuffer[index - 1]);

        GPUReadyBuffer[index++] = normals[i * 3];
        GPUReadyBuffer[index++] = normals[i * 3 + 1];
        GPUReadyBuffer[index++] = normals[i * 3 + 2];
        // printf("NORMAL:  %.3f, %.3f, %.3f\n", i, mesh->GPUReadyBuffer[index - 3], mesh->GPUReadyBuffer[index - 2], mesh->GPUReadyBuffer[index - 1]);

        GPUReadyBuffer[index++] = textureCoordinates[i * 2];
        GPUReadyBuffer[index++] = textureCoordinates[i * 2 + 1];
        // printf("    UV:   %.3f, %.3f\n", mesh->GPUReadyBuffer[index - 2], mesh->GPUReadyBuffer[index - 1]);
    }

    return GPUReadyBuffer;
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
        // cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_cross(camera->front, camera->up, temp[3]);
        glm_normalize(temp[3]);
        glm_vec3_scale(temp[3], cameraSpeed, temp[3]);
        glm_vec3_add(cameraPosition, temp[3], cameraPosition);
        // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    GladGLContext* context = glfwGetWindowUserPointer(window);
    context->Viewport(0, 0, width, height);
    prCameraUpdateDimentions(camera);

    prTextureUpdate(postProcessingTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);

    prTextureUpdate(colorTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);
    prRenderBufferUpdate(depthStencilRBO, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight);

    context->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    prTextureBindImage(postProcessingTexture, 0, 0, PR_ACCESS_WRITE_ONLY, GL_RGBA32F);
    prComputeShaderDispatch(computeShaderProgram, windowWidth, windowHeight, 1);
    context->MemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void framebufferSizeCallbackLite(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    GladGLContext* context = glfwGetWindowUserPointer(window);
    context->Viewport(0, 0, width, height);
    prCameraUpdateDimentions(camera);

    prShaderSetUniform2f(debugShaderProgram, "screenSize", windowWidth, windowHeight);

    prTextureUpdate(colorTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);
    prRenderBufferUpdate(depthStencilRBO, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouseMovement) {
        lastX = xpos;
        lastY = ypos;
        firstMouseMovement = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) {
        pitch = 89.0f;
    }
    if(pitch < -89.0f) {
        pitch = -89.0f;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_X && action == GLFW_PRESS) {
        showHUD = !showHUD;
    }
    if(key == GLFW_KEY_Z && action == GLFW_PRESS) {
        showPostProcessing = !showPostProcessing;
    }

    if(key == GLFW_KEY_1 && action == GLFW_PRESS) {
        currentSkybox = 1;
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS) {
        currentSkybox = 2;
    }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS) {
        currentSkybox = 3;
    }
    if(key == GLFW_KEY_4 && action == GLFW_PRESS) {
        currentSkybox = 4;
    }
    if(key == GLFW_KEY_0 && action == GLFW_PRESS) {
        currentSkybox = 0;
    }

    if(key == GLFW_KEY_BACKSLASH && action == GLFW_PRESS) {
        useDebugShader = !useDebugShader;
    }

    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        GladGLContext* context = glfwGetWindowUserPointer(window);

        GLint viewportSize[4];
        context->GetIntegerv(GL_VIEWPORT, viewportSize);
        int width = viewportSize[2];
        int height = viewportSize[3];

        unsigned char* pixels = prMalloc(width * height * 4);
        context->ReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        int timestamp = time(NULL);
        char name[32];
        snprintf(name, 32, "%i.png", timestamp);

        stbi_flip_vertically_on_write(1);
        stbi_write_png(name, width, height, 4, pixels, width * 4);
        prLogEvent(PR_EVENT_USER, PR_LOG_INFO, "Save screenshot with name: %s", name);

        prFree(pixels);
    }

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void translationsToMatrix(mat4 matrix, vec3 position, vec3 rotation, vec3 scale) {
    glm_mat4_identity(matrix);
    glm_translate(matrix, position);
    glm_rotate(matrix, rotation[2], (vec3){0.0f, 0.0f, 1.0f});
    glm_rotate(matrix, rotation[1], (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(matrix, rotation[0], (vec3){1.0f, 0.0f, 0.0f});
    glm_scale(matrix, scale);
}

float radians(float degrees) {
    return degrees * (M_PI / 180);
}

float smoothOvertime() {
    return (glfwGetTimerValue() * 2.5f) / 5.0f / glfwGetTimerFrequency();
}

float smoothOvertimeSin() {
    return sin((glfwGetTimerValue() * 2.5f) / 5.0f / glfwGetTimerFrequency());
}

prShaderData* createDefaultShader(GladGLContext* context) {
    prShaderData* shader = prShaderCreate();
    prShaderLinkContext(shader, context);
    prShaderUpdate(shader, DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
    return shader;
}

prShaderData* createDebugShader(GladGLContext* context) {
    prShaderData* shader = prShaderCreate();
    prShaderLinkContext(shader, context);
    prShaderUpdate(shader, DEFAULT_VERTEX_SHADER, DEBUG_FRAGMENT_SHADER);
    return shader;
}

void bindMaterial(materialData* material, prShaderData* shaderProgram) {
    prTextureBindTexture(material->ambientMap, 0);
    prTextureBindTexture(material->diffuseMap, 1);
    prTextureBindTexture(material->specularMap, 2);
    prTextureBindTexture(material->normalMap, 3);

    prShaderSetUniform1i(shaderProgram, "material.ambient", 0);
    prShaderSetUniform1i(shaderProgram, "material.diffuse", 1);
    prShaderSetUniform1i(shaderProgram, "material.specular", 2);
    prShaderSetUniform1f(shaderProgram, "material.shininess", material->shininess);
}

void bindMaterialHUD(materialData* material, prShaderData* shaderProgram) {
    prTextureBindTexture(material->ambientMap, 0);

    prShaderSetUniform1i(shaderProgram, "material.ambient", 0);
}

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    printf("---------------\n");
    printf("Debug message ( %i ): %s\n", id, message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             printf("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     printf("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           printf("Source: Other"); break;
    } printf("\n");

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               printf("Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               printf("Type: Other"); break;
    } printf("\n");
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         printf("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       printf("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          printf("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
    } printf("\n");
    printf("\n");
}
