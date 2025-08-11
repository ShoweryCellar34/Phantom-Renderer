#include <globals.hpp>

FILE* g_logFile = NULL;
std::filesystem::path g_logFilePath;

std::filesystem::path g_userDataPath;
std::filesystem::path g_resourcesPath;

bool g_windowInit = false;

prWindow* g_window = NULL;

int windowWidth = DEFAULT_WINDOW_WIDTH;
int windowHeight = DEFAULT_WINDOW_HEIGHT;

float pitch = 0.0f;
float yaw = 0.0f;

int currentSkybox = 0;
bool showHUD = true;
bool showPostProcessing = true;

bool useDebugShader = false;

prShaderData* shaderProgram = NULL;
prShaderData* debugShaderProgram = NULL;
prShaderData* depthShaderProgram = NULL;
prShaderData* depth2ShaderProgram = NULL;
prShaderData* skyboxShaderProgram = NULL;
prShaderData* hudShaderProgram = NULL;
prShaderData* gaussianShaderProgram = NULL;
prShaderData* hdrShaderProgram = NULL;
prComputeShaderData* computeShaderProgram = NULL;

prRenderBufferData* colorRBOMultisampled = NULL;
prTextureMultisampledData* colorMultisamlpedTexture2 = NULL;
prRenderBufferData* depthStencilRBOMultisampled = NULL;
prTextureData* colorTexture2 = NULL;
prTextureData* colorTexture3 = NULL;
prTextureData* bloomTexture = NULL;
prTextureData* postProcessingTexture = NULL;
prTextureData* colorTexture = NULL;
prRenderBufferData* depthStencilRBO = NULL;

prCamera* camera = NULL;
