#include <PR/shader.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <PR/error.h>

unsigned int prShaderGenerateDefaultProgram(GladGLContext* context) {
    const char* vertexShaderSource = "\n\
            #version 330 core\n\
            layout (location = 0) in vec3 inputPosition;\n\
            layout (location = 1) in vec2 inputTextureCoordinates;\n\
            layout (location = 2) in vec4 inputVertexColor;\n\
            layout (location = 3) in vec3 inputVertexNormals;\n\
            \n\
            out vec3 fragmentPosition;\n\
            out vec2 textureCoordinates;\n\
            out vec4 vertexColor;\n\
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
                vertexColor = inputVertexColor;\n\
                vertexNormals = mat3(transpose(inverse(translation))) * inputVertexNormals;\n\
            }\n\
        ";

    const char* fragmentShaderSource = "\n\
            #version 330 core\n\
            out vec4 fragColor;\n\
            \n\
            uniform float mixRatio;\n\
            uniform bool blendAlpha;\n\
            bool alphaSupport = false;\n\
            uniform sampler2D textureSampler;\n\
            \n\
            uniform float ambientStrength;\n\
            uniform float diffuseStrength;\n\
            uniform float specularStrength;\n\
            uniform vec3 cameraPosition;\n\
            \n\
            in vec3 fragmentPosition;\n\
            in vec2 textureCoordinates;\n\
            in vec4 vertexColor;\n\
            in vec3 vertexNormals;\n\
            \n\
            struct Material {\n\
                vec3 ambient;\n\
                vec3 diffuse;\n\
                vec3 specular;\n\
                float shininess;\n\
            };\n\
            uniform Material material;\n\
            \n\
            void main() {\n\
                vec3 lightPos = vec3(0.0, 1.0, 0.0);\n\
                vec3 lightCol = vec3(1.0, 1.0, 1.0);\n\
                \n\
                vec3 ambient = material.ambient * ambientStrength;\n\
                \n\
                vec3 normal = normalize(vertexNormals);\n\
                vec3 lightDirection = normalize(lightPos - fragmentPosition);\n\
                float diff = max(dot(normal, lightDirection), 0.0);\n\
                vec3 diffuse = (diffuseStrength * diff * material.diffuse) * lightCol;\n\
                \n\
                vec3 viewDirection = normalize(cameraPosition - fragmentPosition);\n\
                vec3 reflectDirection = reflect(-lightDirection, normal);\n\
                float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);\n\
                vec3 specular = (specularStrength * spec * material.specular) * lightCol;\n\
                \n\
                vec3 result = (diffuse + specular + ambient);\n\
                \n\
                if(blendAlpha) {\n\
                    vec4 texel = texture(textureSampler, textureCoordinates);\n\
                    fragColor = vec4(mix(vertexColor.xyz, texel.xyz, mixRatio) * result, alphaSupport ? mix(vertexColor.w, texel.w, mixRatio) : 1.0);\n\
                } else {\n\
                    vec4 texel = texture(textureSampler, textureCoordinates);\n\
                    fragColor = vec4(mix(vertexColor.xyz, texel.xyz, mixRatio) * result, alphaSupport ? vertexColor.w + texel.w : 1.0);\n\
                }\n\
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
