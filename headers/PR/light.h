#pragma once

#include <stddef.h>
#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prCamera prCamera;

typedef struct prLightData {
    GladGLContext* context;
    mat4 color;
    GLfloat* GPUReadyBuffer;
    GLuint GPUReadyBufferCount;
} prLightData;

prLightData* prLightCreate();

void prLightDestroy(prLightData* light);

void prLightLinkContext(prLightData* light, GladGLContext* context);

void prLightUpdate(prLightData* light, vec4 color);

void prLightDraw(prLightData* light, mat4 translation, prCamera* camera, unsigned int shaderProgram);
