#pragma once

#include <stdint.h>
#include <stddef.h>

typedef int8_t prLogLevel_t;
typedef int8_t prEvent_t;

#define PR_MAXSTR_LEN    1024

#define PR_EVENT_MEMORY  0
#define PR_EVENT_DATA    1
#define PR_EVENT_OPENGL  2
#define PR_EVENT_USER    3

#define PR_LOG_FATAL     4
#define PR_LOG_ERROR     5
#define PR_LOG_WARNING   6
#define PR_LOG_INFO      7
#define PR_LOG_TRACE     8

#define PR_WRAPPING_REPEAT               GL_REPEAT
#define PR_WRAPPING_REPEAT_MIRRORED      GL_MIRRORED_REPEAT
#define PR_WRAPPING_EDGE                 GL_CLAMP_TO_EDGE
#define PR_WRAPPING_COLOR                GL_CLAMP_TO_BORDER

#define PR_FILTER_LINEAR                 GL_LINEAR
#define PR_FILTER_NEAREST                GL_NEAREST

#define PR_ACCESS_READ_ONLY              GL_READ_ONLY
#define PR_ACCESS_WRITE_ONLY             GL_WRITE_ONLY
#define PR_ACCESS_READ_WRITE             GL_READ_WRITE

#define PR_FORMAT_R                      GL_RED
#define PR_FORMAT_G                      GL_GREEN
#define PR_FORMAT_B                      GL_BLUE
#define PR_FORMAT_A                      GL_ALPHA
#define PR_FORMAT_RGB                    GL_RGB
#define PR_FORMAT_RGBA                   GL_RGBA
#define PR_FORMAT_STENCIL                GL_STENCIL_INDEX
#define PR_FORMAT_DEPTH                  GL_DEPTH_COMPONENT
#define PR_FORMAT_DEPTH_STENCIL          GL_DEPTH24_STENCIL8
#define PR_FORMAT_AUTO                   GL_RGB10_A2UI // Just a random format that can be used as a value for PR_FORMAT_AUTO

#define PR_BUFFER_BIT_COLOR              GL_COLOR_BUFFER_BIT
#define PR_BUFFER_BIT_DEPTH              GL_DEPTH_BUFFER_BIT  
#define PR_BUFFER_BIT_STENCIL            GL_STENCIL_BUFFER_BIT

#ifndef BASE_VERTEX_SHADER
#define BASE_VERTEX_SHADER "\n\
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

#ifndef BASE_FRAGMENT_SHADER
#define BASE_FRAGMENT_SHADER "\n\
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
