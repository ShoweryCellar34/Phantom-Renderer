#pragma once

#define M_PI 3.14159265358979323846
#include <math.h>
#include <GLFW/glfw3.h>
#include <PR/textureInternal.h>
#include "exampleGlobalValues.h"

void proccessInput(GLFWwindow* window, vec3 cameraFront, vec3 cameraUp) {
    mat4x3 temp;
    mat2x3 temp2;
    glm_mat4x3_zero(temp);

    float cameraSpeed = 7.5f * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraSpeed *= 2.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraSpeed /= 2.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_scale(cameraFront, cameraSpeed, temp[0]);
        glm_vec3_add(cameraPosition, temp[0], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_scale(cameraFront, cameraSpeed, temp[1]);
        glm_vec3_sub(cameraPosition, temp[1], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_cross(cameraFront, cameraUp, temp[2]);
        glm_normalize(temp[2]);
        glm_vec3_scale(temp[2], cameraSpeed, temp[2]);
        glm_vec3_sub(cameraPosition, temp[2], cameraPosition);
        // cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_cross(cameraFront, cameraUp, temp[3]);
        glm_normalize(temp[3]);
        glm_vec3_scale(temp[3], cameraSpeed, temp[3]);
        glm_vec3_add(cameraPosition, temp[3], cameraPosition);
        // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glm_vec3_scale(cameraUp, cameraSpeed, temp2[0]);
        glm_vec3_add(cameraPosition, temp2[0], cameraPosition);
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        glm_vec3_scale(cameraUp, cameraSpeed, temp2[1]);
        glm_vec3_sub(cameraPosition, temp2[1], cameraPosition);
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    GladGLContext* context = glfwGetWindowUserPointer(window);
    context->Viewport(0, 0, width, height);
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

prTextureData* loadTexture(GladGLContext* context, const char* path) {
    prTextureData* texture = prTextureCreate();

    FILE* textureFile = fopen(path, "rb");
    if(textureFile == NULL) {
        prLogEvent(PR_USER_EVENT, PR_LOG_EROR, "Failed to load file \"%s\"", path);
        exit(EXIT_FAILURE);
    }

    fseek(textureFile, 0L, SEEK_END);
    size_t textureFileSize = ftell(textureFile);
    fseek(textureFile, 0L, SEEK_SET);
    unsigned char* textureData = prMalloc(textureFileSize + 1);
    fread(textureData, textureFileSize, 1, textureFile);
    fclose(textureFile);

    prTextureLinkContext(texture, context);
    prTextureUpdate(texture, PR_WRAPPING_EDGE, textureData, textureFileSize);
    prFree(textureData);

    return texture;
}

prTextureData* makeTextureSingleColor(GladGLContext* context, float* color) {
    prTextureData* texture = prTextureCreate();
    prTextureLinkContext(texture, context);

    texture->textureData = prMalloc(4 * sizeof(GLubyte));
    texture->textureData[0] = color[0] * 255.0f;
    texture->textureData[1] = color[1] * 255.0f;
    texture->textureData[2] = color[2] * 255.0f;
    texture->textureData[3] = color[3] * 255.0f;
    texture->channels = 4;
    texture->height = 1;
    texture->width = 1;

    i_prTextureCreateOnGPU(texture);

    return texture;
}

prTextureData* makeTextureDefault(GladGLContext* context, size_t scale) {
    prTextureData* texture = prTextureCreate();
    prTextureLinkContext(texture, context);

    bool* template = prMalloc(scale * scale * sizeof(bool));
    for(size_t i = 0; i < scale * scale; i++) {
        static bool offset = false;
        if(!(i % scale) && !(scale % 2)) {
            offset = !offset;
        }

        if((i + offset) % 2) {
            template[i] = false;
        } else {
            template[i] = true;
        }
    }

    texture->textureData = prMalloc(scale * scale * 4 * sizeof(GLubyte));
    texture->wrappingMode = PR_WRAPPING_REPEAT;
    texture->pixelated = true;

    for(size_t i = 0; i < scale * scale; i++) {
        size_t index = i * 4;
        texture->textureData[index++] = (template[i] ? 255 : 0);
        texture->textureData[index++] = 0;
        texture->textureData[index++] = (template[i] ? 255 : 0);
        texture->textureData[index] = 255;
    }

    prFree(template);

    texture->channels = 4;
    texture->height = scale;
    texture->width = scale;

    i_prTextureCreateOnGPU(texture);

    return texture;
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
