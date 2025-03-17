#include <PR/light.h>

#include <PR/memory.h>
#include <PR/camera.h>
#include <PR/error.h>

prLightData* prLightCreate() {
    prLightData* light = prMalloc(sizeof(prLightData));

    glm_vec4_zero(light->color);

    return light;
}

void prLightDestroy(prLightData* light) {
    prFree(light);
}

void prLightUpdate(prLightData* light, vec4 color) {
    glm_vec4_copy(color, light->color);
}
