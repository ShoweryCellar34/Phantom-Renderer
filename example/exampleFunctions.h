#pragma once

#include <GLFW/glfw3.h>
#include "exampleGlobalValues.h"

void proccessInput(GLFWwindow* window, vec3 cameraFront, vec3 cameraUp) {
    mat4x3 temp;
    glm_mat4x3_zero(temp);

    float cameraSpeed = 7.5f * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraSpeed *= 2.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraSpeed /= 2.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_scale(cameraFront, cameraSpeed, temp[0]);
        glm_vec3_add(cameraPosition, temp[0], cameraPosition);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_scale(cameraFront, cameraSpeed, temp[1]);
        glm_vec3_sub(cameraPosition, temp[1], cameraPosition);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_cross(cameraFront, cameraUp, temp[2]);
        glm_normalize(temp[2]);
        glm_vec3_scale(temp[2], cameraSpeed, temp[2]);
        glm_vec3_sub(cameraPosition, temp[2], cameraPosition);
        // cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_cross(cameraFront, cameraUp, temp[3]);
        glm_normalize(temp[3]);
        glm_vec3_scale(temp[3], cameraSpeed, temp[3]);
        glm_vec3_add(cameraPosition, temp[3], cameraPosition);
        // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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

void loadTexture(prTextureData* texture, GladGLContext* context, const char* path) {
    FILE* textureFile = fopen(path, "rb");
    fseek(textureFile, 0L, SEEK_END);
    size_t textureFileSize = ftell(textureFile);
    fseek(textureFile, 0L, SEEK_SET);
    unsigned char* textureData = prMalloc(textureFileSize + 1);
    fread(textureData, textureFileSize, 1, textureFile);
    fclose(textureFile);

    prTextureLink(texture, context);
    prTextureUpdate(texture, textureData, textureFileSize);
    prFree(textureData);
}
