#pragma once

#include <stdio.h>
#include <filesystem>
#include <PR/PR.h>
#include <materials.hpp>

#define TO_USR(x) (g_userDataPath / x).u8string().c_str()
#define TO_RES(x) (g_resourcesPath / x).u8string().c_str()

// Global Constants
#define TITLE "Phantom-Renderer Example"
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define SAMPLES 4
#define BLOOM_BLUR_PASSES 6

#define POINT_LIGHT_RESOLUTION 2048
#define SUN_LIGHT_WIDTH 4096
#define SUN_LIGHT_HEIGHT 4096

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
extern prShaderData* g_shaderDirectionalLight;
extern prShaderData* g_shaderPointLight;
extern prShaderData* g_shaderSkybox;
extern prShaderData* g_shaderHUD;
extern prShaderData* g_shaderGaussianBlur;
extern prShaderData* g_shaderHDR;
extern prComputeShaderData* g_computeShaderPostProcessing;
extern prTextureData* g_texturePostProcessing;

// Framebuffers
extern bool g_framebuffersInit;

extern prTextureData* g_textureDepthSunShadowMap;
extern prFramebufferData* g_framebufferSunShadowMap;
extern prCubeMapData* g_cubeMapDepthPointShadowMap;
extern prFramebufferData* g_framebufferPointShadowMap;
extern prRenderBufferData* g_RBOColorMultisampled;
extern prTextureMultisampledData* g_multisampledTextureColorMultisampled;
extern prRenderBufferData* g_multisampledRBODepthStencilMultisampled;
extern prTextureData* g_textureColorGaussianBlur1;
extern prFramebufferData* g_framebufferGaussianBlur1;
extern prTextureData* g_textureColorGaussianBlur2;
extern prFramebufferData* g_framebufferGaussianBlur2;
extern prFramebufferData* g_gaussianBlurFramebuffers[2];
extern prTextureData* g_gaussianBlurTextures[2];
extern prTextureData* g_textureBloom;
extern prFramebufferData* g_framebufferBloom;
extern prFramebufferData* g_framebufferMultisampled;
extern prTextureData* g_textureColorDefault;
extern prRenderBufferData* g_RBODepthStencilDefault;
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

extern prCubeMapData* g_cubeMapBlack;
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
extern vec3s cameraPosition;

extern float deltaTime;
extern float lastFrame;
