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
bool g_textureInit = false;

prTextureData* defaultTexture = nullptr;
prTextureData* containerTexture = nullptr;
prTextureData* containerMetalTexture = nullptr;
prTextureData* containerMetalSpecularTexture = nullptr;
prTextureData* steelTexture = nullptr;
prTextureData* steelNormal = nullptr;
prTextureData* brickWallDiffuseTexture = nullptr;
prTextureData* brickWallNormalTexture = nullptr;
prTextureData* blackTexture = nullptr;
prTextureData* whiteTexture = nullptr;
prTextureData* defaultNormal = nullptr;
prTextureData* HUDTexture = nullptr;

float pitch = 0.0f;
float yaw = 0.0f;

int currentSkybox = 0;
bool showHUD = true;
bool showPostProcessing = true;

bool useDebugShader = false;

prCamera* camera = nullptr;
