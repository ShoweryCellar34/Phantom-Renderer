#pragma once

#include <stdio.h>
#include <filesystem>
#include <PR/PR.h>

#define TO_RES(x) (g_resourcesPath / x).u8string().c_str()

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

extern int windowWidth;
extern int windowHeight;

// Shaders
extern bool g_shadersInit;

extern prShaderData* shaderProgram;
extern prShaderData* debugShaderProgram;
extern prShaderData* depthShaderProgram;
extern prShaderData* depth2ShaderProgram;
extern prShaderData* skyboxShaderProgram;
extern prShaderData* hudShaderProgram;
extern prShaderData* gaussianShaderProgram;
extern prShaderData* hdrShaderProgram;
extern prComputeShaderData* computeShaderProgram;

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
extern bool g_textureInit;

extern prTextureData* defaultTexture;
extern prTextureData* containerTexture;
extern prTextureData* containerMetalTexture;
extern prTextureData* containerMetalSpecularTexture;
extern prTextureData* steelTexture;
extern prTextureData* steelNormal;
extern prTextureData* brickWallDiffuseTexture;
extern prTextureData* brickWallNormalTexture;
extern prTextureData* blackTexture;
extern prTextureData* whiteTexture;
extern prTextureData* defaultNormal;
extern prTextureData* HUDTexture;

extern float pitch;
extern float yaw;

extern int currentSkybox;
extern bool showHUD;
extern bool showPostProcessing;

extern bool useDebugShader;

extern prCamera* camera;
