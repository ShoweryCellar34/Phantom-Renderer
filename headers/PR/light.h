#pragma once

#include <stddef.h>
#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prLightData {
    vec4 color;
} prLightData;

prLightData* prLightCreate();

void prLightDestroy(prLightData* light);

void prLightUpdate(prLightData* light, vec4 color);
