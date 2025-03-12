#pragma once

#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prCamera {
    GladGLContext* context;
    mat4 view;
    mat4 projection;
    int viewportWidth, viewportHeight;
    vec3 front;
    vec3 up;
} prCamera;

prCamera* prCameraCreate();

void prCameraDestroy(prCamera* camera);

void prCameraLink(prCamera* camera, GladGLContext* context);

void prCameraUpdate(prCamera* camera, vec3 position, vec3 rotation);
