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

    prTextureUpdate(postProcessingTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);

    prTextureUpdate(colorTexture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, PR_FILTER_LINEAR, NULL, 0, windowWidth, windowHeight);
    prRenderBufferUpdate(depthStencilRBO, PR_FORMAT_DEPTH_STENCIL, windowWidth, windowHeight);

    context->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    prTextureBindImage(postProcessingTexture, 0, 0, PR_ACCESS_WRITE_ONLY, GL_RGBA32F);
    prComputeShaderDispatch(computeShaderProgram, windowWidth, windowHeight, 1);
    context->MemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
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

static const char* DEFAULT_VERTEX_SHADER = 
    "#version 450 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "    TexCoord = aTexCoord;\n"
    "    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
    "}\n";

static const char* DEFAULT_FRAGMENT_SHADER = 
    "#version 450 core\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "struct DirectionalLight {\n"
    "    vec3 direction;\n"
    "    vec3 ambient;\n"
    "    vec3 diffuse;\n"
    "    vec3 specular;\n"
    "};\n"
    "struct PointLight {\n"
    "    vec3 position;\n"
    "    float constant;\n"
    "    float linear;\n"
    "    float quadratic;\n"
    "    vec3 ambient;\n"
    "    vec3 diffuse;\n"
    "    vec3 specular;\n"
    "};\n"
    "uniform DirectionalLight directionalLights[4];\n"
    "uniform PointLight pointLights[4];\n"
    "uniform sampler2D texture0;\n"  // Simplified texture handling
    "uniform vec3 viewPos;\n"
    "void main() {\n"
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 viewDir = normalize(viewPos - FragPos);\n"
    "    vec3 result = vec3(0.0);\n"
    "    // Directional light\n"
    "    vec3 lightDir = normalize(-directionalLights[0].direction);\n"
    "    float diff = max(dot(norm, lightDir), 0.0);\n"
    "    vec3 reflectDir = reflect(-lightDir, norm);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);\n"
    "    result += directionalLights[0].ambient * vec3(texture(texture0, TexCoord));\n"
    "    result += diff * directionalLights[0].diffuse * vec3(texture(texture0, TexCoord));\n"
    "    result += spec * directionalLights[0].specular * vec3(texture(texture0, TexCoord));\n"
    "    // Point light\n"
    "    vec3 pointLightDir = normalize(pointLights[0].position - FragPos);\n"
    "    float pointDiff = max(dot(norm, pointLightDir), 0.0);\n"
    "    vec3 pointReflectDir = reflect(-pointLightDir, norm);\n"
    "    float pointSpec = pow(max(dot(viewDir, pointReflectDir), 0.0), 32.0);\n"
    "    float distance = length(pointLights[0].position - FragPos);\n"
    "    float attenuation = 1.0 / (pointLights[0].constant + pointLights[0].linear * distance + pointLights[0].quadratic * distance * distance);\n"
    "    result += attenuation * (pointLights[0].ambient * vec3(texture(texture0, TexCoord)) +\n"
    "                            pointDiff * pointLights[0].diffuse * vec3(texture(texture0, TexCoord)) +\n"
    "                            pointSpec * pointLights[0].specular * vec3(texture(texture0, TexCoord)));\n"
    "    FragColor = vec4(result, 1.0);\n"
    "}\n";

prShaderData* createDefaultShader(GladGLContext* context) {
    prShaderData* shader = prShaderCreate();
    prShaderLinkContext(shader, context);
    prShaderUpdate(shader, DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
    return shader;
}
