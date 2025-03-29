#pragma once

#include <cglm/cglm.h>

typedef struct prDirectionalLightData {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} prDirectionalLightData;

prDirectionalLightData* prDirectionalLightCreate();

void prDirectionalLightDestroy(prDirectionalLightData* light);

void prDirectionalLightSetDirection(prDirectionalLightData* light, vec3 direction);

void prDirectionalLightSetAmbient(prDirectionalLightData* light, vec3 abmient);

void prDirectionalLightSetDiffuse(prDirectionalLightData* light, vec3 diffuse);

void prDirectionalLightSetSpecular(prDirectionalLightData* light, vec3 specular);

typedef struct prPointLightData {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} prPointLightData;

prPointLightData* prPointLightCreate();

void prPointLightDestroy(prPointLightData* light);

void prPointLightSetPosition(prPointLightData* light, vec3 position);

void prPointLightSetAmbient(prPointLightData* light, vec3 abmient);

void prPointLightSetDiffuse(prPointLightData* light, vec3 diffuse);

void prPointLightSetSpecular(prPointLightData* light, vec3 specular);
