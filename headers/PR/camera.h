#pragma once

#include <PR/defines.h>

#include <glad/gl.h>
#include <cglm/struct.h>

typedef struct prCamera {
    GladGLContext* context;
    PRCameraUpdateFunction updateFunction;
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

void prCameraSetUpdateFunction(prCamera* camera, PRCameraUpdateFunction updateFunction);

void prCameraUpdate(prCamera* camera, vec3s position, vec3s rotation, vec3s worldUp, float FOV, float closePlane, float farPlane);

void prCameraUpdateDimentions(prCamera* camera);
