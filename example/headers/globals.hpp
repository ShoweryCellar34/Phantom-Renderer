#pragma once

#include <stdio.h>
#include <filesystem>
#include <PR/PR.h>
#include <materials.hpp>

#define TO_RES(x) (g_resourcesPath / x).u8string().c_str()

inline GLfloat* TEMP_RGBA(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    static GLfloat tempRGBA[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    tempRGBA[0] = r;
    tempRGBA[1] = g;
    tempRGBA[2] = b;
    tempRGBA[3] = a;

    return tempRGBA;
}

inline GLfloat (*TEMP_RGBA_6(GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                             GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                             GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3,
                             GLfloat r4, GLfloat g4, GLfloat b4, GLfloat a4,
                             GLfloat r5, GLfloat g5, GLfloat b5, GLfloat a5,
                             GLfloat r6, GLfloat g6, GLfloat b6, GLfloat a6
))[4] {
    static GLfloat tempRGBA[PR_CUBE_MAP_SIDES][4] = {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}};

    tempRGBA[0][0] = r1;
    tempRGBA[0][1] = g1;
    tempRGBA[0][2] = b1;
    tempRGBA[0][3] = a1;

    tempRGBA[1][0] = r2;
    tempRGBA[1][1] = g2;
    tempRGBA[1][2] = b2;
    tempRGBA[1][3] = a2;

    tempRGBA[2][0] = r3;
    tempRGBA[2][1] = g3;
    tempRGBA[2][2] = b3;
    tempRGBA[2][3] = a3;

    tempRGBA[3][0] = r4;
    tempRGBA[3][1] = g4;
    tempRGBA[3][2] = b4;
    tempRGBA[3][3] = a4;

    tempRGBA[4][0] = r5;
    tempRGBA[4][1] = g5;
    tempRGBA[4][2] = b5;
    tempRGBA[4][3] = a5;

    tempRGBA[5][0] = r6;
    tempRGBA[5][1] = g6;
    tempRGBA[5][2] = b6;
    tempRGBA[5][3] = a6;

    return tempRGBA;
}

// Global Constants
#define TITLE "Phantom-Renderer Example"
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define SAMPLES 4

// Logging
extern FILE* g_logFile;
extern std::filesystem::path g_logFilePath;

// Paths
extern std::filesystem::path g_userDataPath;
extern std::filesystem::path g_resourcesPath;

//Windowing
extern bool g_windowInit;

extern prWindow* g_window;

extern int g_windowWidth;
extern int g_windowHeight;

// Shaders
extern bool g_shadersInit;

extern prShaderData* g_shaderDefault;
extern prShaderData* g_shaderDebug;
extern prShaderData* g_shaderDirectionLight;
extern prShaderData* g_shaderPointLight;
extern prShaderData* g_shaderSkybox;
extern prShaderData* g_shaderHUD;
extern prShaderData* g_shaderGaussianBlur;
extern prShaderData* g_shaderHDR;
extern prComputeShaderData* g_computeShaderPostProcessing;
extern prTextureData* g_texturePostProcessing;

// Framebuffers
extern bool g_framebuffersInit;

extern prTextureData* g_depthTextureSunShadowMap;
extern prFramebufferData* g_framebufferSunShadowMap;
extern prCubeMapData* g_depthCubeMapPointShadowMap;
extern prFramebufferData* g_framebufferPointShadowMap;
extern prRenderBufferData* g_colorRBOMultisampled;
extern prTextureMultisampledData* g_colorMultisamlpedTextureMultisampled;
extern prRenderBufferData* g_depthStencilMultisampledRBOMultisampled;
extern prTextureData* g_colorTextureGaussian1;
extern prFramebufferData* g_framebufferGaussian1;
extern prTextureData* g_colorTextureGaussian2;
extern prFramebufferData* g_framebufferGaussian2;
extern prFramebufferData* gaussianFramebuffers[2];
extern prTextureData* gaussianTextures[2];
extern prTextureData* g_bloomTexture;
extern prFramebufferData* g_framebufferBloom;
extern prFramebufferData* g_framebufferMultisampled;
extern prTextureData* g_colorTextureDefault;
extern prRenderBufferData* g_depthStencilRBODefault;
extern prFramebufferData* g_framebufferDefault;

// Textures
extern bool g_texturesInit;

extern prTextureData* g_textureCheckerboard;
extern prTextureData* g_textureBlack;
extern prTextureData* g_textureWhite;
extern prTextureData* g_textureNormalDefault;

extern prTextureData* g_textureHUD;

extern prTextureData* g_textureContainer;
extern prTextureData* g_textureMetalRimmedContainer;
extern prTextureData* g_textureMetalRimmedContainerSpecular;
extern prTextureData* g_textureSteel;
extern prTextureData* g_textureSteelNormal;
extern prTextureData* g_textureBrickWall;
extern prTextureData* g_textureBrickWallNormal;

extern prCubeMapData* skyboxDefaultCubeMap;
extern prCubeMapData* g_cubeMapIslands;
extern prCubeMapData* g_cubeMapSpace;
extern prCubeMapData* g_cubeMap3;
extern prCubeMapData* g_cubeMap4;

// Materials
extern bool g_materialsInit;

extern material g_materialCheckerboard;
extern material g_materialBlack;
extern material g_materialWhite;

extern material g_materialHUD;
extern material g_materialPostProcessing;

extern material g_materialContainer;
extern material g_materialMetalRimmedContainer;
extern material g_materialSteel;
extern material g_materialBrickWall;

// Meshes
extern bool g_meshesInit;

extern prMeshData* g_meshCube;
extern prMeshData* g_meshQuad;

extern float pitch;
extern float yaw;

extern int currentSkybox;
extern bool showHUD;
extern bool showPostProcessing;

extern bool useDebugShader;

extern prCamera* camera;
