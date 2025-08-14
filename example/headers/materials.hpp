#pragma once

#include <PR/PR.h>

class material {
private:
    prTextureData* diffuseMap;
    prTextureData* specularMap;
    prTextureData* normalMap;

    GLfloat shininess;
public:
    material();
    material(prTextureData* diffuseMap, prTextureData* specularMap, prTextureData* normalMap, GLfloat shininess);
    ~material();

    void setMaps(prTextureData* diffuseMap, prTextureData* specularMap, prTextureData* normalMap, GLfloat shininess);
    void setDiffuseMap(prTextureData* diffuseMap);
    void setSpecularMap(prTextureData* specularMap);
    void setNormalMap(prTextureData* normalMap);
    void setShininess(GLfloat shininess);

    prTextureData* getDiffuseMap();
    prTextureData* getSpecularMap();
    prTextureData* getNormalMap();
    GLfloat getShininess();

    void bind(prShaderData* shader);
};
