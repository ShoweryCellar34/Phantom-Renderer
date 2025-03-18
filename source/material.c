#include <PR/material.h>

#include <PR/memory.h>

prMaterialData* prMaterialCreate() {
    prMaterialData* material = prMalloc(sizeof(prMaterialData));

    glm_vec3_zero(material->ambient);
    glm_vec3_zero(material->diffuse);
    glm_vec3_zero(material->specular);
    material->shininess = 0.0f;
    material->ambientStrength = 0.0f;
    material->diffuseStrength = 0.0f;
    material->specularStrength = 0.0f;

    return material;
}

prMaterialData* prMaterialCreateDefaults() {
    prMaterialData* material = prMalloc(sizeof(prMaterialData));

    glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, material->ambient);
    glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, material->diffuse);
    glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, material->specular);
    material->shininess = 32.0f;
    material->ambientStrength = 0.15f;
    material->diffuseStrength = 1.0f;
    material->specularStrength = 0.5f;

    return material;
}

void prMaterialDestroy(prMaterialData* material) {
    prFree(material);
}

void prMaterialSetAmbient(prMaterialData* material, vec3 ambient) {
    glm_vec3_copy(ambient, material->ambient);
}

void prMaterialSetDiffuse(prMaterialData* material, vec3 diffuse) {
    glm_vec3_copy(diffuse, material->diffuse);
}

void prMaterialSetSpecular(prMaterialData* material, vec3 specular) {
    glm_vec3_copy(specular, material->specular);
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
