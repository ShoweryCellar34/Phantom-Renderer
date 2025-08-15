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

int g_windowWidth = DEFAULT_WINDOW_WIDTH;
int g_windowHeight = DEFAULT_WINDOW_HEIGHT;

// Shaders
bool g_shadersInit = false;

prShaderData* g_shaderDefault = nullptr;
prShaderData* g_shaderDebug = nullptr;
prShaderData* g_shaderDirectionLight = nullptr;
prShaderData* g_shaderPointLight = nullptr;
prShaderData* g_shaderSkybox = nullptr;
prShaderData* g_shaderHUD = nullptr;
prShaderData* g_shaderGaussianBlur = nullptr;
prShaderData* g_shaderHDR = nullptr;
prComputeShaderData* g_computeShaderPostProcessing = nullptr;
prTextureData* g_texturePostProcessing = nullptr;

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
prTextureData* g_textureBlack = nullptr;
prTextureData* g_textureWhite = nullptr;
prTextureData* g_textureNormalDefault = nullptr;

prTextureData* g_textureHUD = nullptr;

prTextureData* g_textureContainer = nullptr;
prTextureData* g_textureMetalRimmedContainer = nullptr;
prTextureData* g_textureMetalRimmedContainerSpecular = nullptr;
prTextureData* g_textureSteel = nullptr;
prTextureData* g_textureSteelNormal = nullptr;
prTextureData* g_textureBrickWall = nullptr;
prTextureData* g_textureBrickWallNormal = nullptr;

prCubeMapData* skyboxDefaultCubeMap = nullptr;
prCubeMapData* g_cubeMapIslands = nullptr;
prCubeMapData* g_cubeMapSpace = nullptr;
prCubeMapData* g_cubeMap3 = nullptr;
prCubeMapData* g_cubeMap4 = nullptr;

// Materials
bool g_materialsInit = false;

material g_materialCheckerboard;
material g_materialBlack;
material g_materialWhite;

material g_materialHUD;
material g_materialPostProcessing;

material g_materialContainer;
material g_materialMetalRimmedContainer;
material g_materialSteel;
material g_materialBrickWall;

// Meshes
bool g_meshesInit = false;

prMeshData* g_meshCube = nullptr;
prMeshData* g_meshQuad = nullptr;

float pitch = 0.0f;
float yaw = 0.0f;

int currentSkybox = 0;
bool showHUD = true;
bool showPostProcessing = true;

bool useDebugShader = false;

prCamera* camera = nullptr;
