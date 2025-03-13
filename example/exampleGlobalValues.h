#pragma once

float vertices[] = {
    0.6f,  0.6f, 0.6f, // top right
    0.6f, -0.6f, 0.3f, // bottom right
   -0.6f, -0.6f, 0.6f, // bottom left
   -0.6f,  0.6f, 0.3f  // top left
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
    0.0f, 0.0f, 0.0f, 0.0f
};

unsigned int indices[] = {
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

vec3 cameraPosition = {0.0f, 0.0f, 0.0f};
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouseMovement = true;
float pitch = 0.0f;
float yaw = 270.0f;
double lastX = 0.0f;
double lastY = 0.0f;
