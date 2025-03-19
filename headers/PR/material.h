#pragma once

#include <cglm/cglm.h>

typedef struct prTextureData prTextureData;

typedef struct prMaterialData {
    prTextureData* texture;
    prTextureData* ambientMap;
    prTextureData* diffuseMap;
    prTextureData* specularMap;
    float shininess;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
} prMaterialData;

prMaterialData* prMaterialCreate();

prMaterialData* prMaterialCreateDefaults();

void prMaterialDestroy(prMaterialData* material);

void prMaterialLinkTexture(prMaterialData* material, prTextureData* texture);

void prMaterialLinkAmbientMap(prMaterialData* material, prTextureData* ambientMap);

void prMaterialLinkDiffuseMap(prMaterialData* material, prTextureData* diffuseMap);

void prMaterialLinkSpecularMap(prMaterialData* material, prTextureData* specularMap);

void prMaterialSetShininess(prMaterialData* material, float shininess);

void prMaterialSetAmbientStrength(prMaterialData* material, float ambientStrength);

void prMaterialSetDiffuseStrength(prMaterialData* material, float diffuseStrength);

void prMaterialSetSpecularStrength(prMaterialData* material, float specularStrength);
