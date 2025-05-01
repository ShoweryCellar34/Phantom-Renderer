#pragma once

#define M_PI 3.14159265358979323846
#include <math.h>
#include <GLFW/glfw3.h>
#include "exampleGlobalValues.h"

void proccessInput(GLFWwindow* window) {
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

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    GladGLContext* context = glfwGetWindowUserPointer(window);
    context->Viewport(0, 0, width, height);
    prCameraUpdateDimentions(camera);

    prTextureUpdate(colorTexture, PR_FORMAT_RGBA, PR_FILTER_LINEAR, PR_WRAPPING_EDGE, NULL, 0, windowWidth, windowHeight);
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
