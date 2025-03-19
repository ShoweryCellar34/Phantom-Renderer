#include <PR/shader.h>

#include <PR/defines.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <PR/error.h>

unsigned int prShaderGenerateDefaultProgram(GladGLContext* context) {
    const char* vertexShaderSource = "\n\
            #version 330 core\n\
            layout (location = 0) in vec3 inputPosition;\n\
            layout (location = 1) in vec2 inputTextureCoordinates;\n\
            layout (location = 2) in vec3 inputVertexNormals;\n\
            \n\
            out vec3 fragmentPosition;\n\
            out vec2 textureCoordinates;\n\
            out vec3 vertexNormals;\n\
            \n\
            uniform mat4 translation;\n\
            uniform mat4 view;\n\
            uniform mat4 projection;\n\
            \n\
            void main() {\n\
                gl_Position = projection * view * translation * vec4(inputPosition, 1.0);\n\
                fragmentPosition = vec3(translation * vec4(inputPosition, 1.0));\n\
                textureCoordinates = inputTextureCoordinates;\n\
                vertexNormals = mat3(transpose(inverse(translation))) * inputVertexNormals;\n\
            }\n\
        ";

    const char* fragmentShaderSource = "\n\
            #version 330 core\n\
            out vec4 fragmentColor;\n\
            \n\
            uniform float mixRatio;\n\
            uniform bool blendAlpha;\n\
            bool alphaSupport = false;\n\
            \n\
            uniform vec3 cameraPosition;\n\
            \n\
            in vec3 fragmentPosition;\n\
            in vec2 textureCoordinates;\n\
            in vec3 vertexNormals;\n\
            \n\
            struct Material {\n\
                sampler2D ambient;\n\
                sampler2D diffuse;\n\
                sampler2D specular;\n\
                float shininess;\n\
            };\n\
            uniform Material material;\n\
            \n\
            struct Light {\n\
                int type;\n\
                vec3 position;\n\
                vec3 direction;\n\
                vec4 color;\n\
                float constant;\n\
                float linear;\n\
                float quadratic;\n\
                float cutOff;\n\
            };\n\
            \n\
            void main() {\n\
                Light light = Light(1, vec3(0.0, 0.0, 10.0), vec3(0.0, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0), 1.0, 0.045, 0.0075, 45.0);\n\
                vec3 lightDirection;\n\
                switch(light.type) {\n\
                    case 0:\n\
                        lightDirection = normalize(-light.direction);\n\
                        break;\n\
                    \n\
                    case 1:\n\
                        lightDirection = normalize(light.position - fragmentPosition);\n\
                        break;\n\
                }\n\
                \n\
                float distance = length(light.position - fragmentPosition);\n\
                float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));\n\
                \n\
                vec4 ambient = texture(material.ambient, textureCoordinates) * 0.15 * light.color;\n\
                \n\
                vec3 normal = normalize(vertexNormals);\n\
                float diff = max(dot(normal, lightDirection), 0.0);\n\
                vec4 diffuse = texture(material.diffuse, textureCoordinates) * diff * light.color * attenuation;\n\
                \n\
                vec3 viewDirection = normalize(cameraPosition - fragmentPosition);\n\
                vec3 reflectDirection = reflect(-lightDirection, normal);\n\
                float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);\n\
                vec4 specular = texture(material.specular, textureCoordinates) * spec * light.color * attenuation;\n\
                \n\
                vec4 result = (diffuse + specular + ambient);\n\
                \n\
                fragmentColor = vec4(result.xyz, alphaSupport ? result.w : 1.0);\n\
            }\n\
        ";

    int success;
    char infoLog[512];
    char failMessage[768];

    unsigned int vertexShader = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    context->CompileShader(vertexShader);

    context->GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Vertex shader 0 failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    unsigned int fragmentShader = context->CreateShader(GL_FRAGMENT_SHADER);
    context->ShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    context->CompileShader(fragmentShader);

    context->GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Fragment shader failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    unsigned int shaderProgram = context->CreateProgram();
    context->AttachShader(shaderProgram, vertexShader);
    context->AttachShader(shaderProgram, fragmentShader);
    context->LinkProgram(shaderProgram);

    context->GetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        context->GetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Shader program failed to link. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        context->DeleteProgram(shaderProgram);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    return shaderProgram;
}
