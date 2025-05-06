#pragma once

#include <glad/gl.h>

typedef struct prTextureData prTextureData;

typedef struct prMaterialData {
    prTextureData* ambientMap;
    prTextureData* diffuseMap;
    prTextureData* specularMap;
    prTextureData* normalMap;
    GLfloat shininess;
} prMaterialData;

prMaterialData* prMaterialCreate();

prMaterialData* prMaterialCreateDefaults();

void prMaterialDestroy(prMaterialData* material);

void prMaterialLinkAmbientMap(prMaterialData* material, prTextureData* ambientMap);

void prMaterialLinkDiffuseMap(prMaterialData* material, prTextureData* diffuseMap);

void prMaterialLinkSpecularMap(prMaterialData* material, prTextureData* specularMap);

void prMaterialLinkNormalMap(prMaterialData* material, prTextureData* normalMap);

void prMaterialSetShininess(prMaterialData* material, GLfloat shininess);
