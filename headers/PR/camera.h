#pragma once

#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prCamera {
    GladGLContext* context;
    vec3 position;
    mat4 view;
    mat4 projection;
    int viewportWidth, viewportHeight;
    vec3 front;
    vec3 up;
} prCamera;

prCamera* prCameraCreate();

void prCameraDestroy(prCamera* camera);

void prCameraLinkContext(prCamera* camera, GladGLContext* context);

void prCameraUpdate(prCamera* camera, vec3 position, vec3 rotation, float FOV, float closePlane, float farPlane);
