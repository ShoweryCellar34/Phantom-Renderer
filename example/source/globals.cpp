#include <globals.hpp>

// Logging
FILE* g_logFile = nullptr;
std::filesystem::path g_logFilePath;

// Paths
std::filesystem::path g_userDataPath;
std::filesystem::path g_resourcesPath;

// Windows
bool g_windowInit = false;

prWindow* g_window = nullptr;

int windowWidth = DEFAULT_WINDOW_WIDTH;
int windowHeight = DEFAULT_WINDOW_HEIGHT;

// Shaders
bool g_shadersInit = false;

prShaderData* shaderProgram = nullptr;
prShaderData* debugShaderProgram = nullptr;
prShaderData* depthShaderProgram = nullptr;
prShaderData* depth2ShaderProgram = nullptr;
prShaderData* skyboxShaderProgram = nullptr;
prShaderData* hudShaderProgram = nullptr;
prShaderData* gaussianShaderProgram = nullptr;
prShaderData* hdrShaderProgram = nullptr;
prComputeShaderData* computeShaderProgram = nullptr;

// Framebuffers
bool g_framebuffersInit = false;

prTextureData* g_depthTextureSunShadowMap = nullptr;
prFramebufferData* g_framebufferSunShadowMap = nullptr;
prCubeMapData* g_depthCubeMapPointShadowMap = nullptr;
prFramebufferData* g_framebufferPointShadowMap = nullptr;
prRenderBufferData* g_colorRBOMultisampled = nullptr;
prTextureMultisampledData* g_colorMultisamlpedTextureMultisampled = nullptr;
prRenderBufferData* g_depthStencilMultisampledRBOMultisampled = nullptr;
prTextureData* g_colorTextureGaussian1 = nullptr;
prFramebufferData* g_framebufferGaussian1 = nullptr;
prTextureData* g_colorTextureGaussian2 = nullptr;
prFramebufferData* g_framebufferGaussian2 = nullptr;
prFramebufferData* gaussianFramebuffers[2] = {nullptr};
prTextureData* gaussianTextures[2] = {nullptr};
prTextureData* g_bloomTexture = nullptr;
prFramebufferData* g_framebufferBloom = nullptr;
prFramebufferData* g_framebufferMultisampled = nullptr;
prTextureData* g_colorTextureDefault = nullptr;
prRenderBufferData* g_depthStencilRBODefault = nullptr;
prFramebufferData* g_framebufferDefault = nullptr;

// Textures
bool g_texturesInit = false;

prTextureData* g_textureCheckerboard = nullptr;
prTextureData* g_textureContainer = nullptr;
prTextureData* g_textureMetalRimmedContainer = nullptr;
prTextureData* g_textureMetalRimmedContainerSpecular = nullptr;
prTextureData* g_textureSteel = nullptr;
prTextureData* g_textureSteelNormal = nullptr;
prTextureData* g_textureBrickWall = nullptr;
prTextureData* g_textureBrickWallNormal = nullptr;
prTextureData* g_textureBlack = nullptr;
prTextureData* g_textureWhite = nullptr;
prTextureData* g_textureNormalDefault = nullptr;
prTextureData* g_textureHUD = nullptr;
prCubeMapData* skyboxDefaultCubeMap = nullptr;
prCubeMapData* g_cubeMapIslands = nullptr;
prCubeMapData* g_cubeMapSpace = nullptr;
prCubeMapData* skyboxCubeMap3 = nullptr;
prCubeMapData* skybox4CubeMap = nullptr;

float pitch = 0.0f;
float yaw = 0.0f;

int currentSkybox = 0;
bool showHUD = true;
bool showPostProcessing = true;

bool useDebugShader = false;

prCamera* camera = nullptr;
