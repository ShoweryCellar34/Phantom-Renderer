#include <PR/light.h>

#include <PR/memory.h>
#include <PR/camera.h>
#include <PR/logger.h>

prDirectionalLightData* prDirectionalLightCreate() {
    prDirectionalLightData* light = prMalloc(sizeof(prDirectionalLightData));

    glm_vec3_zero(light ->direction);
    glm_vec3_one(light->ambient);
    glm_vec3_one(light->diffuse);
    glm_vec3_one(light->specular);

    return light;
}

void prDirectionalLightDestroy(prDirectionalLightData* light) {
    prFree(light);
}

void prDirectionalLightSetDirection(prDirectionalLightData* light, vec3 direction) {
    glm_vec3_copy(direction, light->direction);
}

void prDirectionalLightSetAmbient(prDirectionalLightData* light, vec3 ambient) {
    glm_vec3_copy(ambient, light->ambient);
}

void prDirectionalLightSetDiffuse(prDirectionalLightData* light, vec3 diffuse) {
    glm_vec3_copy(diffuse, light->diffuse);
}

void prDirectionalLightSetSpecular(prDirectionalLightData* light, vec3 specular) {
    glm_vec3_copy(specular, light->specular);
}

prPointLightData* prPointLightCreate() {
    prPointLightData* light = prMalloc(sizeof(prPointLightData));

    glm_vec3_zero(light ->position);
    light->constant = 0.0f;
    light->linear = 0.0f;
    light->quadratic = 0.0f;
    glm_vec3_one(light->ambient);
    glm_vec3_one(light->diffuse);
    glm_vec3_one(light->specular);

    return light;
}

void prPointLightDestroy(prPointLightData* light) {
    prFree(light);
}

void prPointLightSetPosition(prPointLightData* light, vec3 position) {
    glm_vec3_copy(position, light->position);
}

void prPointLightSetAmbient(prPointLightData* light, vec3 ambient) {
    glm_vec3_copy(ambient, light->ambient);
}

void prPointLightSetDiffuse(prPointLightData* light, vec3 diffuse) {
    glm_vec3_copy(diffuse, light->diffuse);
}

void prPointLightSetSpecular(prPointLightData* light, vec3 specular) {
    glm_vec3_copy(specular, light->specular);
}
