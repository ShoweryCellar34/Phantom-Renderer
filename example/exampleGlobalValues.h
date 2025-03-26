#pragma once

const char* vertexShader = "\n\
#version 330 core\n\
layout (location = 0) in vec3 inputPosition;\n\
layout (location = 1) in vec2 inputTextureCoordinates;\n\
\n\
out vec3 fragmentPosition;\n\
out vec2 textureCoordinates;\n\
\n\
uniform mat4 translation;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
\n\
void main() {\n\
    gl_Position = projection * view * translation * vec4(inputPosition, 1.0);\n\
    fragmentPosition = vec3(translation * vec4(inputPosition, 1.0));\n\
    textureCoordinates = inputTextureCoordinates;\n\
}\n\
";

const char* fragmentShader = "\n\
#version 330 core\n\
out vec4 fragmentColor;\n\
\n\
uniform vec3 cameraPosition;\n\
uniform mat4 translation;\n\
\n\
in vec3 fragmentPosition;\n\
in vec2 textureCoordinates;\n\
in mat3 TBN;\n\
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
    vec3 ambient = texture(material.ambient, textureCoordinates).xyz;\n\
    vec3 diffuse = texture(material.diffuse, textureCoordinates).xyz;\n\
    vec3 specular = texture(material.specular, textureCoordinates).www;\n\
    vec3 normal = normalize(mat3(transpose(inverse(translation))) * texture(material.normal, textureCoordinates).xyz);\n\
    normal = normal * 2.0 - 1.0;\n\
    normal = normalize(TBN * normal);\n\
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
    fragmentColor = vec4(result, 1.0);\n\
}\n\
";

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

unsigned int indices[] = {
    0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
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

vec3 cameraPosition = {0.0f, 0.0f, 5.0f};
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouseMovement = true;
float pitch = 0.0f;
float yaw = 270.0f;
double lastX = 0.0f;
double lastY = 0.0f;
