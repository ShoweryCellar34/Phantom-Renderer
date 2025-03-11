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
            \n\
            out vec2 textureCoordinates;\n\
            out vec4 vertexColor;\n\
            \n\
            uniform mat4 translation;\n\
            uniform mat4 view;\n\
            uniform mat4 projection;\n\
            \n\
            void main() {\n\
                gl_Position = projection * view * translation * vec4(inputPosition, 1.0);\n\
                textureCoordinates = inputTextureCoordinates;\n\
                vertexColor = inputVertexColor;\n\
            }\n\
        ";

    const char* fragmentShaderSource = "\n\
            #version 330 core\n\
            out vec4 fragColor;\n\
            \n\
            uniform float mixRatio;\n\
            uniform bool blendAlpha;\n\
            uniform bool alphaSupport = false;\n\
            \n\
            in vec2 textureCoordinates;\n\
            in vec4 vertexColor;\n\
            \n\
            uniform sampler2D textureSampler;\n\
            \n\
            void main() {\n\
                if(blendAlpha) {\n\
                    vec4 texel = texture(textureSampler, textureCoordinates);\n\
                    fragColor = vec4(mix(vertexColor.xyz, texel.xyz, mixRatio), alphaSupport ? mix(vertexColor.w, texel.w, mixRatio) : 1.0);\n\
                } else {\n\
                    vec4 texel = texture(textureSampler, textureCoordinates);\n\
                    fragColor = vec4(mix(vertexColor.xyz, texel.xyz, mixRatio), alphaSupport ? vertexColor.w + texel.w : 1.0);\n\
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
