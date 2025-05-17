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

#ifndef DEFAULT_VERTEX_SHADER
#define DEFAULT_VERTEX_SHADER "\n\
#version 460 core\n\
layout (location = 0) in vec3 inputPosition;\n\
layout (location = 1) in vec3 inputNormals;\n\
layout (location = 2) in vec2 inputTextureCoordinates;\n\
\n\
out vec3 fragmentPosition;\n\
out vec3 normals;\n\
out vec2 textureCoordinates;\n\
\n\
uniform mat4 translation;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
\n\
void main() {\n\
    gl_Position = projection * view * translation * vec4(inputPosition, 1.0);\n\
    fragmentPosition = vec3(translation * vec4(inputPosition, 1.0));\n\
    normals = mat3(transpose(inverse(translation))) * inputNormals;\n\
    textureCoordinates = inputTextureCoordinates;\n\
}\n\
"
#endif

#ifndef DEFAULT_FRAGMENT_SHADER
#define DEFAULT_FRAGMENT_SHADER "\n\
#version 460 core\n\
out vec4 fragmentColor;\n\
\n\
uniform vec3 cameraPosition;\n\
uniform mat4 translation;\n\
\n\
in vec3 fragmentPosition;\n\
in vec3 normals;\n\
in vec2 textureCoordinates;\n\
\n\
struct Material {\n\
    sampler2D ambient;\n\
    sampler2D diffuse;\n\
    sampler2D specular;\n\
    sampler2D normal;\n\
    float shininess;\n\
};\n\
uniform Material material;\n\
\n\
struct directionalLight {\n\
    vec3 direction;\n\
    \n\
    vec3 ambient;\n\
    vec3 diffuse;\n\
    vec3 specular;\n\
};\n\
#define NR_DIRECTIONAL_LIGHTS 1\n\
uniform directionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];\n\
\n\
struct pointLight {\n\
    vec3 position;\n\
    \n\
    float constant;\n\
    float linear;\n\
    float quadratic;\n\
    \n\
    vec3 ambient;\n\
    vec3 diffuse;\n\
    vec3 specular;\n\
};\n\
#define NR_POINT_LIGHTS 1\n\
uniform pointLight pointLights[NR_POINT_LIGHTS];\n\
\n\
vec3 calculateDirectionalLight(directionalLight light, vec3 viewDirection, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal) {\n\
    vec3 lightDir = normalize(-light.direction);\n\
    \n\
    float difference = max(dot(normal, lightDir), 0.0);\n\
    \n\
    vec3 reflectDir = reflect(-lightDir, normal);\n\
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);\n\
    \n\
    vec3 ambientOutput  = light.ambient * ambient;\n\
    vec3 diffuseOutput  = light.diffuse * difference * diffuse;\n\
    vec3 specularOutput = light.specular * spec * specular;\n\
    \n\
    return (ambientOutput + diffuseOutput + specularOutput);\n\
}\n\
\n\
vec3 calculatePointLight(pointLight light, vec3 viewDirection, vec3 fragmentPosition, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal) {\n\
    vec3 lightDir = normalize(light.position - fragmentPosition);\n\
    \n\
    float difference = max(dot(normal, lightDir), 0.0);\n\
    \n\
    vec3 reflectDirection = reflect(-lightDir, normal);\n\
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);\n\
    \n\
    float distance = length(light.position - fragmentPosition);\n\
    float attenuation = 1.0 / (light.constant + light.linear * distance + \n\
                light.quadratic * (distance * distance));    \n\
    \n\
    vec3 ambientOutput = light.ambient * ambient;\n\
    vec3 diffuseOutput = light.diffuse * difference * diffuse;\n\
    vec3 specularOutput = light.specular * spec * specular;\n\
    ambientOutput *= attenuation;\n\
    diffuseOutput *= attenuation;\n\
    specularOutput *= attenuation;\n\
    \n\
    return (ambientOutput + diffuseOutput + specularOutput);\n\
}\n\
\n\
void main() {\n\
    vec4 sampledTexture = texture(material.ambient, textureCoordinates);\n\
    vec3 ambient = sampledTexture.rgb;\n\
    vec3 diffuse = texture(material.diffuse, textureCoordinates).rgb;\n\
    vec3 specular = texture(material.specular, textureCoordinates).rgb;\n\
    vec3 normal = normalize(texture(material.normal, textureCoordinates).rgb);\n\
    normal = normalize(normals);\n\
    \n\
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);\n\
    \n\
    vec3 result = vec3(0.0, 0.0, 0.0);\n\
    \n\
    for(int i = 0; i < NR_DIRECTIONAL_LIGHTS; i++) {\n\
        result += calculateDirectionalLight(directionalLights[i], viewDirection, ambient, diffuse, specular, normal);\n\
    }\n\
    \n\
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {\n\
        result += calculatePointLight(pointLights[i], viewDirection, fragmentPosition, ambient, diffuse, specular, normal);\n\
    }\n\
    \n\
    fragmentColor = vec4(result, sampledTexture.a);\n\
}\n\
"
#endif

#ifndef SKYBOX_VERTEX_SHADER
#define SKYBOX_VERTEX_SHADER "\n\
#version 460 core\n\
layout (location = 0) in vec3 inputPosition;\n\
\n\
out vec3 textureCoordinates;\n\
\n\
uniform mat4 translation;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
\n\
void main() {\n\
    gl_Position = (projection * view * translation * vec4(inputPosition, 1.0)).xyww;\n\
    textureCoordinates = inputPosition;\n\
}\n\
"
#endif

#ifndef SKYBOX_FRAGMENT_SHADER
#define SKYBOX_FRAGMENT_SHADER "\n\
#version 460 core\n\
out vec4 fragmentColor;\n\
\n\
in vec3 textureCoordinates;\n\
\n\
layout (location = 0) uniform samplerCube skybox;\n\
\n\
void main() {\n\
    fragmentColor = texture(skybox, textureCoordinates);\n\
}\n\
"
#endif

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
layout (binding = 0) uniform sampler2D inputImage;\n\
layout (rgba32f, binding = 1) uniform image2D outputImage;\n\
\n\
const float offset = 1.0 / 300.0;\n\
\n\
void main() {\n\
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);\n\
    vec2 uv = vec2(pixelCoord) / vec2(imageSize(outputImage));\n\
    \n\
    vec2 offsets[9] = vec2[](\n\
        vec2(-offset,  offset), // top-left\n\
        vec2( 0.0f,    offset), // top-center\n\
        vec2( offset,  offset), // top-right\n\
        vec2(-offset,  0.0f),   // center-left\n\
        vec2( 0.0f,    0.0f),   // center-center\n\
        vec2( offset,  0.0f),   // center-right\n\
        vec2(-offset, -offset), // bottom-left\n\
        vec2( 0.0f,   -offset), // bottom-center\n\
        vec2( offset, -offset)  // bottom-right\n\
    );\n\
    \n\
    float kernel[9] = float[](\n\
        -1, -1, -1,\n\
        -1,  9, -1,\n\
        -1, -1, -1\n\
    );\n\
    \n\
    vec4 samples[9];\n\
    for(int i = 0; i < 9; i++) {\n\
        samples[i] = texture(inputImage, uv + offsets[i]);\n\
    }\n\
    \n\
    vec4 finalSample = vec4(0.0);\n\
    for(int i = 0; i < 9; i++) {\n\
        finalSample += samples[i] * kernel[i];\n\
    }\n\
    \n\
    imageStore(outputImage, pixelCoord, finalSample);\n\
}\n\
"

const char* skyboxTextures[6] = {
    "res/skybox/right.jpg",
    "res/skybox/left.jpg",
    "res/skybox/top.jpg",
    "res/skybox/bottom.jpg",
    "res/skybox/front.jpg",
    "res/skybox/back.jpg",
};

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
