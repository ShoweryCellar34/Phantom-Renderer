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
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f
};

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

float verticesQuad[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f
};

float normalsQuad[] = {
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
};

float textureCoordinatesQuad[] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
};

unsigned int indicesQuad[] = {
    0, 1, 2,
    2, 3, 0
};

prCamera* camera = NULL;
vec3 cameraPosition = {0.0f, 0.0f, 5.0f};
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouseMovement = true;
float pitch = 0.0f;
float yaw = 270.0f;
double lastX = 0.0f;
double lastY = 0.0f;

#define HUD_VERTEX_SHADER "\n\
#version 460 core\n\
layout (location = 0) in vec3 inputPosition;\n\
layout (location = 2) in vec2 inputTextureCoordinates;\n\
\n\
out vec2 textureCoordinates;\n\
\n\
void main() {\n\
    vec4 pos = vec4(inputPosition, 1.0);\n\
    pos.xy = pos.xy * 2.0 - 1.0;\n\
    gl_Position = pos;\n\
    textureCoordinates = inputTextureCoordinates;\n\
}\n\
"

#define HUD_FRAGMENT_SHADER "\n\
#version 460 core\n\
out vec4 fragmentColor;\n\
\n\
in vec2 textureCoordinates;\n\
\n\
struct Material {\n\
    sampler2D ambient;\n\
};\n\
uniform Material material;\n\
\n\
void main() {\n\
    fragmentColor = texture(material.ambient, textureCoordinates);\n\
}\n\
"

#define HUD_COMPUTE_SHADER "\n\
#version 460 core\n\
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;\n\
layout (rgba32f, binding = 0) uniform image2D outputImage;\n\
void main() {\n\
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);\n\
    vec2 uv = vec2(pixelCoord) / vec2(imageSize(outputImage));\n\
    vec3 color = vec3(uv, 0.5);\n\
    imageStore(outputImage, pixelCoord, vec4(color, 0.5));\n\
}\n\
"

int showHUD = 1;
int showPostProcessing = 1;

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
