#pragma once

#include <stdio.h>
#include <filesystem>
#include <PR/PR.h>

#define TITLE "Phantom-Renderer Example"
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define SAMPLES 4

extern FILE* g_logFile;
extern std::filesystem::path g_logFilePath;

extern std::filesystem::path g_userDataPath;
extern std::filesystem::path g_resourcesPath;

extern bool g_windowInit;

extern prWindow* g_window;

extern int windowWidth;
extern int windowHeight;

extern float pitch;
extern float yaw;

extern int currentSkybox;
extern bool showHUD;
extern bool showPostProcessing;

extern bool useDebugShader;

extern prShaderData* shaderProgram;
extern prShaderData* debugShaderProgram;
extern prShaderData* depthShaderProgram;
extern prShaderData* depth2ShaderProgram;
extern prShaderData* skyboxShaderProgram;
extern prShaderData* hudShaderProgram;
extern prShaderData* gaussianShaderProgram;
extern prShaderData* hdrShaderProgram;
extern prComputeShaderData* computeShaderProgram;

extern prRenderBufferData* colorRBOMultisampled;
extern prTextureMultisampledData* colorMultisamlpedTexture2;
extern prRenderBufferData* depthStencilRBOMultisampled;
extern prTextureData* colorTexture2;
extern prTextureData* colorTexture3;
extern prTextureData* bloomTexture;
extern prTextureData* postProcessingTexture;
extern prTextureData* colorTexture;
extern prRenderBufferData* depthStencilRBO;

extern prCamera* camera;
