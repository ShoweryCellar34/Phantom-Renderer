#pragma once

float vertices[] = {
   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,

   -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,

   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f, -0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,

   -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f
};

float normals[] = {
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    0.0f, 0.0f,  1.0f,
    0.0f, 0.0f,  1.0f,
    0.0f, 0.0f,  1.0f,
    0.0f, 0.0f,  1.0f,
    0.0f, 0.0f,  1.0f,
    0.0f, 0.0f,  1.0f,

   -1.0f, 0.0f,  0.0f,
   -1.0f, 0.0f,  0.0f,
   -1.0f, 0.0f,  0.0f,
   -1.0f, 0.0f,  0.0f,
   -1.0f, 0.0f,  0.0f,
   -1.0f, 0.0f,  0.0f,

    1.0f, 0.0f,  0.0f,
    1.0f, 0.0f,  0.0f,
    1.0f, 0.0f,  0.0f,
    1.0f, 0.0f,  0.0f,
    1.0f, 0.0f,  0.0f,
    1.0f, 0.0f,  0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f,  1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    0.0f,  1.0f, 0.0f 
};

float textureCoordinates[] = {
    // Back face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    // Front face
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    // Left face
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,

    // Right face
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,

    // Bottom face
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,

    // Top face
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
};
void* cubeData = NULL;
int cubeDataSize = 0;

unsigned int indices[] = {
    0, 2, 1,
    3, 5, 4,

    6, 7, 8,
    9, 10, 11,

    12, 13, 14,
    15, 16, 17,

    18, 20, 19,
    21, 23, 22,

    24, 25, 26,
    27, 28, 29,

    30, 32, 31,
    33, 35, 34
};
int indicesSize = sizeof(indices);

float quadData[] = {
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};
int quadDataSize = sizeof(quadData);

unsigned int indicesQuad[] = {
    0, 1, 2,
    2, 3, 0
};
int indicesQuadSize = sizeof(indicesQuad);

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

prCamera* camera = NULL;
vec3 cameraPosition = {0.0f, 0.0f, 5.0f};
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouseMovement = true;
float pitch = 0.0f;
float yaw = 270.0f;
double lastX = 0.0f;
double lastY = 0.0f;

const char* skybox1Textures[6] = {
    "res/skyboxes/1/right.jpg",
    "res/skyboxes/1/left.jpg",
    "res/skyboxes/1/top.jpg",
    "res/skyboxes/1/bottom.jpg",
    "res/skyboxes/1/front.jpg",
    "res/skyboxes/1/back.jpg",
};

const char* skybox2Textures[6] = {
    "res/skyboxes/2/px.png",
    "res/skyboxes/2/nx.png",
    "res/skyboxes/2/py.png",
    "res/skyboxes/2/ny.png",
    "res/skyboxes/2/pz.png",
    "res/skyboxes/2/nz.png",
};

int currentSkybox = 1;
bool showHUD = 1;
bool showPostProcessing = 1;

bool useDebugShader = false;
prShaderData* debugShaderProgram = NULL;
prComputeShaderData* computeShaderProgram = NULL;
prTextureData* postProcessingTexture = NULL;
prTextureData* colorTexture = NULL;
prRenderBufferData* depthStencilRBO = NULL;
prFramebufferData* framebuffer = NULL;

#define DEF_WIDTH 1280
#define DEF_HEIGHT 720
int windowWidth = DEF_WIDTH;
int windowHeight = DEF_HEIGHT;
#define TITLE "Phantom-Renderer Demo"
