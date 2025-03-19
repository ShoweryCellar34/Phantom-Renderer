#include <PR/material.h>

#include <PR/defines.h>

#include <PR/memory.h>
#include <PR/texture.h>
#include <PR/error.h>

prMaterialData* prMaterialCreate() {
    prMaterialData* material = prMalloc(sizeof(prMaterialData));

    material->texture = NULL;
    material->ambientMap = NULL;
    material->diffuseMap = NULL;
    material->specularMap = NULL;
    material->shininess = 0.0f;
    material->ambientStrength = 0.0f;
    material->diffuseStrength = 0.0f;
    material->specularStrength = 0.0f;

    return material;
}

prMaterialData* prMaterialCreateDefaults() {
    prMaterialData* material = prMalloc(sizeof(prMaterialData));

    material->texture = NULL;
    material->specularMap = NULL;
    material->ambientMap = NULL;
    material->diffuseMap = NULL;
    material->specularMap = NULL;
    material->shininess = 32.0f;
    material->ambientStrength = 0.15f;
    material->diffuseStrength = 1.0f;
    material->specularStrength = 0.5f;

    return material;
}

void prMaterialDestroy(prMaterialData* material) {
    prFree(material);
}

void prMaterialLinkTexture(prMaterialData* material, prTextureData* texture) {
    material->texture = texture;
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

void prMaterialSetShininess(prMaterialData* material, float shininess) {
    material->shininess = shininess;
}

void prMaterialSetAmbientStrength(prMaterialData* material, float ambientStrength) {
    material->ambientStrength = ambientStrength;
}

void prMaterialSetDiffuseStrength(prMaterialData* material, float diffuseStrength) {
    material->diffuseStrength = diffuseStrength;
}

void prMaterialSetSpecularStrength(prMaterialData* material, float specularStrength) {
    material->specularStrength = specularStrength;
}
