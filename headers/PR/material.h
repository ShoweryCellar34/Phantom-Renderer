#pragma once

#include <cglm/cglm.h>

typedef struct prMaterialData {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
} prMaterialData;

prMaterialData* prMaterialCreate();

prMaterialData* prMaterialCreateDefaults();

void prMaterialDestroy(prMaterialData* material);

void prMaterialSetAmbient(prMaterialData* material, vec3 ambient);

void prMaterialSetDiffuse(prMaterialData* material, vec3 diffuse);

void prMaterialSetSpecular(prMaterialData* material, vec3 specular);

void prMaterialSetShininess(prMaterialData* material, float shininess);
