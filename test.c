#include <PR/PR.h>

#include <stdio.h>
#include <cglm/cglm.h>
#include <PR/memory.h>

float vertices[] = {
    0.6f,  0.6f, 0.6f, // top right
    0.6f, -0.6f, 0.3f, // bottom right
   -0.6f, -0.6f, 0.6f, // bottom left
   -0.6f,  0.6f, 0.3f  // top left
};

float vertices2[] = {
    1.0f,  1.0f, 0.0f, // top right
    1.0f, -0.0f, 1.0f, // bottom right
   -0.0f, -0.0f, 0.0f, // bottom left
   -0.0f,  1.0f, -1.0f  // top left
};

float vertices3[] = {
    0.0f,  0.0f, 0.2f, // top right
    0.0f, -1.0f, -0.4f, // bottom right
   -1.0f, -1.0f, 0.4f, // bottom left
   -1.0f,  0.0f, -0.2f  // top left
};

float textureCoordinates[] = {
    1.0f,  1.0f, // top right
    1.0f,  0.0f, // bottom right
    0.0f,  0.0f, // bottom left
    0.0f, 1.0f   // top left
};

float vertexColor[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    GladGLContext* context = glfwGetWindowUserPointer(window);
    context->Viewport(0, 0, width, height);
}

vec3 cameraPosition;
vec3 cameraFront;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    vec3 cameraUp;
    cameraUp[0] = 0.0f;
    cameraUp[1] = 1.0f;
    cameraUp[2] = 0.0f;

    mat4x3 temp;
    glm_mat4x3_zero(temp);

    const float cameraSpeed = 7.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) != GLFW_RELEASE) {
        glm_vec3_scale(cameraFront, cameraSpeed, temp[0]);
        glm_vec3_add(cameraPosition, temp[0], cameraPosition);
    }

    if (glfwGetKey(window, GLFW_KEY_S) != GLFW_RELEASE) {
        glm_vec3_scale(cameraFront, cameraSpeed, temp[1]);
        glm_vec3_sub(cameraPosition, temp[1], cameraPosition);
    }

    if (glfwGetKey(window, GLFW_KEY_A) != GLFW_RELEASE) {
        glm_cross(cameraFront, cameraUp, temp[2]);
        glm_normalize(temp[2]);
        glm_vec3_scale(temp[2], cameraSpeed, temp[2]);
        glm_vec3_sub(cameraPosition, temp[2], cameraPosition);
        // cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) != GLFW_RELEASE) {
        glm_cross(cameraFront, cameraUp, temp[3]);
        glm_normalize(temp[3]);
        glm_vec3_scale(temp[3], cameraSpeed, temp[3]);
        glm_vec3_add(cameraPosition, temp[3], cameraPosition);
        // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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

int main(int argc, char** argv) {
    prSetLogLevel(PR_LOG_LEVEL_TRACE);
    prStartFileLogging();

    glfwInit();

    prWindow* test = prWindowCreate("Test", 600, 600);
    prWindowInitContext(test);
    glfwSetWindowUserPointer(test->window, test->openglContext);

    prEnableBlending(test->openglContext);
    test->openglContext->Enable(GL_DEPTH_TEST);
    prEnableImageFlip();

    unsigned int shaderProgram = prShaderGenerateDefaultProgram(test->openglContext);

    prTextureData* testTexture = prTextureCreate();
    loadTexture(testTexture, test->openglContext, "res/awesomeface.png");

    prTextureData* testTexture2 = prTextureCreate();
    loadTexture(testTexture2, test->openglContext, "res/container.jpg");

    prMeshData* testMesh = prMeshCreate();
    prMeshLinkWindow(testMesh, test->openglContext);
    prMeshLinkTexture(testMesh, testTexture);
    prMeshUpdate(testMesh, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float), vertexColor, sizeof(vertexColor) / sizeof(float));
    prMeshTextureToColorRatio(testMesh, 0.5f);

    prMeshData* testMesh2 = prMeshCreate();
    prMeshLinkWindow(testMesh2, test->openglContext);
    prMeshLinkTexture(testMesh2, testTexture2);
    prMeshUpdate(testMesh2, vertices2, sizeof(vertices2) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float), NULL, 0);
    prMeshTextureToColorRatio(testMesh2, 1.0f);

    prMeshData* testMesh3 = prMeshCreate();
    prMeshLinkWindow(testMesh3, test->openglContext);
    prMeshUpdate(testMesh3, vertices3, sizeof(vertices3) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), NULL, 0, vertexColor, sizeof(vertexColor) / sizeof(float));
    prMeshTextureToColorRatio(testMesh3, 0.0f);

    prCamera* camera = prCameraCreate();
    prCameraLink(camera, test->openglContext);

    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);
    glfwSetKeyCallback(test->window, keyCallback);

    glm_vec3_zero(cameraPosition);
    cameraFront[0] = 0.0f;
    cameraFront[1] = 0.0f;
    cameraFront[2] = -1.0f;

    while(!glfwWindowShouldClose(test->window)) {
        prWindowClear(test->openglContext);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        mat4 translation;
        glm_mat4_identity(translation);
        vec3 offset;
        offset[0] = 0.0f;
        offset[1] = 0.0f;
        offset[2] = -3.0f;
        glm_translate(translation, offset);

        prCameraUpdate(camera, cameraPosition);

        int viewUniformLocation = test->openglContext->GetUniformLocation(shaderProgram, "view");
        test->openglContext->UniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, camera->view[0]);

        int projectionUniformLocation = test->openglContext->GetUniformLocation(shaderProgram, "projection");
        test->openglContext->UniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, camera->projection[0]);

        prMeshDraw(testMesh2, translation, shaderProgram);
        prMeshDraw(testMesh3, translation, shaderProgram);
        prMeshDraw(testMesh, translation, shaderProgram);

        glfwSwapBuffers(test->window);

        glfwPollEvents();
    }

    prCameraDestroy(camera);
    camera = NULL;

    prMeshDestroy(testMesh);
    testMesh = NULL;
    prMeshDestroy(testMesh2);
    testMesh2 = NULL;
    prMeshDestroy(testMesh3);
    testMesh3 = NULL;

    prTextureDestroy(testTexture);
    testTexture = NULL;
    prTextureDestroy(testTexture2);
    testTexture2 = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    prEndFileLogging();
}
