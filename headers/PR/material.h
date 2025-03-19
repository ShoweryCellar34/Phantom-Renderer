#pragma once

#include <cglm/cglm.h>

typedef struct prTextureData prTextureData;

typedef struct prMaterialData {
    prTextureData* ambientMap;
    prTextureData* diffuseMap;
    prTextureData* specularMap;
    float shininess;
} prMaterialData;

prMaterialData* prMaterialCreate();

prMaterialData* prMaterialCreateDefaults();

void prMaterialDestroy(prMaterialData* material);

void prMaterialLinkAmbientMap(prMaterialData* material, prTextureData* ambientMap);

void prMaterialLinkDiffuseMap(prMaterialData* material, prTextureData* diffuseMap);

void prMaterialLinkSpecularMap(prMaterialData* material, prTextureData* specularMap);

void prMaterialSetShininess(prMaterialData* material, float shininess);
