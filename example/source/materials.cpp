#include <materials.hpp>

material::material() {
}

material::material(prTextureData* diffuseMap, prTextureData* specularMap, prTextureData* normalMap, GLfloat shininess) {
    this->diffuseMap = diffuseMap;
    this->specularMap = specularMap;
    this->normalMap = normalMap;

    this->shininess = shininess;
}

material::~material() {
}

void material::setMaps(prTextureData* diffuseMap, prTextureData* specularMap, prTextureData* normalMap, GLfloat shininess) {
    this->diffuseMap = diffuseMap;
    this->specularMap = specularMap;
    this->normalMap = normalMap;

    this->shininess = shininess;
}

void material::setDiffuseMap(prTextureData* diffuseMap) {
    this->diffuseMap = diffuseMap;
}

void material::setSpecularMap(prTextureData* specularMap) {
    this->specularMap = specularMap;
}

void material::setNormalMap(prTextureData* normalMap) {
    this->normalMap = normalMap;
}

void material::setShininess(GLfloat shininess) {
    this->shininess = shininess;
}

prTextureData* material::getDiffuseMap() {
    return this->diffuseMap;
}

prTextureData* material::getSpecularMap() {
    return this->specularMap;
}

prTextureData* material::getNormalMap() {
    return this->normalMap;
}

GLfloat material::getShininess() {
    return this->shininess;
}

void material::bind(prShaderData* shader) {
    if(prShaderGetUniformLocation(shader, "material.diffuse") != -1 && this->diffuseMap) {
        prTextureBindTexture(this->diffuseMap, 0);
        prShaderSetUniform1i(shader, "material.diffuse", 0);
    }
    if(prShaderGetUniformLocation(shader, "material.specular") != -1 && this->specularMap) {
        prTextureBindTexture(this->specularMap, 1);
        prShaderSetUniform1i(shader, "material.specular", 1);
    }
    if(prShaderGetUniformLocation(shader, "material.normal") != -1 && this->normalMap) {
        prTextureBindTexture(this->normalMap, 2);
        prShaderSetUniform1i(shader, "material.normal", 2);
    }
    if(prShaderGetUniformLocation(shader, "material.shininess") != -1) {
        prShaderSetUniform1f(shader, "material.shininess", this->shininess);
    }
}
