#pragma once

#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prTextureData prTextureData;
typedef struct prCamera prCamera;
typedef struct prShaderData prShaderData;
typedef struct prComputeShaderData prComputeShaderData;
typedef struct prRenderBufferData prRenderBufferData;
typedef struct prTextureMultisampledData prTextureMultisampledData;
typedef struct prFramebufferData prFramebufferData;

typedef struct materialData {
    prTextureData* ambientMap;
    prTextureData* diffuseMap;
    prTextureData* specularMap;
    prTextureData* normalMap;
    GLfloat shininess;
} materialData;

typedef struct skyboxMaterialData {
    prTextureData* rightTexture;
    prTextureData* leftTexture;
    prTextureData* topTexture;
    prTextureData* bottomTexture;
    prTextureData* backTexture;
    prTextureData* frontTexture;
} skyboxMaterialData;

typedef struct directionalLightData {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    GLuint shadowMap;
} directionalLightData;

typedef struct pointLightData {
    float constant;
    float linear;
    float quadratic;

    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    GLuint shadowMap;
    float farPlane;
} pointLightData;

extern float cubeData[];
extern int cubeDataSize;

extern unsigned int indices[];
extern int indicesSize;

extern float quadData[];
extern int quadDataSize;

extern unsigned int indicesQuad[];
extern int indicesQuadSize;

extern prCamera* camera;
extern vec3 cameraPosition;
extern float deltaTime;
extern float lastFrame;

extern bool firstMouseMovement;
extern float pitch;
extern float yaw;
extern double lastX;
extern double lastY;

extern const char* skybox1Textures[6];

extern const char* skybox2Textures[6];

extern int currentSkybox;
extern bool showHUD;
extern bool showPostProcessing;

extern bool useDebugShader;
extern prShaderData* debugShaderProgram;
extern prComputeShaderData* computeShaderProgram;
extern prRenderBufferData* colorRBOMultisampled;
extern prTextureMultisampledData* colorMultisamlpedTexture2;
extern prRenderBufferData* depthStencilRBOMultisampled;
extern prTextureData* colorTexture2;
extern prTextureData* colorTexture3;
extern prTextureData* bloomTexture;
extern prFramebufferData* framebufferMultisampled;
extern prTextureData* postProcessingTexture;
extern prTextureData* colorTexture;
extern prRenderBufferData* depthStencilRBO;
extern prFramebufferData* framebuffer;

#define BLOOM_BLUR_PASSES 6
#define SAMPLES 4
#define DEF_WIDTH 1280
#define DEF_HEIGHT 720
extern int windowWidth;
extern int windowHeight;
#define TITLE "Phantom-Renderer Demo"
