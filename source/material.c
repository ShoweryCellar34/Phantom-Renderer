#include <PR/material.h>

#include <PR/defines.h>

#include <PR/memory.h>
#include <PR/texture.h>
#include <PR/logger.h>

prMaterialData* prMaterialCreate() {
    prMaterialData* material = prCalloc(1, sizeof(prMaterialData));

    return material;
}

void prMaterialDestroy(prMaterialData* material) {
    prFree(material);
}

void prMaterialLinkAmbientMap(prMaterialData* material, prTextureData* ambientMap) {
    material->ambientMap = ambientMap;
}

void prMaterialLinkDiffuseMap(prMaterialData* material, prTextureData* diffuseMap) {
    material->diffuseMap = diffuseMap;
}

void prMaterialLinkSpecularMap(prMaterialData* material, prTextureData* specularMap) {
    material->specularMap = specularMap;
}

void prMaterialLinkNormalMap(prMaterialData* material, prTextureData* normalMap) {
    material->normalMap = normalMap;
}

void prMaterialSetShininess(prMaterialData* material, float shininess) {
    material->shininess = shininess;
}
