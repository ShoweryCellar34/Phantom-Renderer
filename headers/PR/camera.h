#pragma once

#include <glad/gl.h>
#include <cglm/struct.h>

typedef struct prCamera {
    GladGLContext* context;
    vec3s position;
    mat4s view;
    mat4s projection;
    GLint viewportWidth, viewportHeight;
    vec3s front;
    vec3s up;
} prCamera;

prCamera* prCameraCreate();

void prCameraDestroy(prCamera* camera);

void prCameraLinkContext(prCamera* camera, GladGLContext* context);

void prCameraUpdate(prCamera* camera, vec3s position, vec3s rotation, float FOV, float closePlane, float farPlane);

void prCameraUpdateDimentions(prCamera* camera);
