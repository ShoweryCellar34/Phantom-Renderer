#include <PR/shader.h>

#include <stddef.h>
#include <stdio.h>
#include <PR/error.h>

unsigned int prShaderGenerateDefaultProgram(GladGLContext* context) {
    const char *vertexShaderSourceTextureOnly = "\n\
            #version 330 core\n\
            layout (location = 0) in vec3 inputPosition;\n\
            layout (location = 1) in vec2 inputTextureCoordinates;\n\
            \n\
            out vec2 textureCoordinates;\n\
            out vec4 vertexColor;\n\
            \n\
            void main() {\n\
                gl_Position = vec4(inputPosition, 1.0);\n\
                textureCoordinates = inputTextureCoordinates;\n\
            }\n\
        ";

    const char *vertexShaderSourceColorOnly = "\n\
            #version 330 core\n\
            layout (location = 0) in vec3 inputPosition;\n\
            layout (location = 1) in vec4 inputVertexColor;\n\
            \n\
            out vec2 textureCoordinates;\n\
            out vec4 vertexColor;\n\
            \n\
            void main() {\n\
                gl_Position = vec4(inputPosition, 1.0);\n\
                vertexColor = inputVertexColor;\n\
            }\n\
        ";

    const char *vertexShaderSourceTextureAndColor = "\n\
            #version 330 core\n\
            layout (location = 0) in vec3 inputPosition;\n\
            layout (location = 1) in vec2 inputTextureCoordinates;\n\
            layout (location = 2) in vec4 inputVertexColor;\n\
            \n\
            out vec2 textureCoordinates;\n\
            out vec4 vertexColor;\n\
            \n\
            void main() {\n\
                gl_Position = vec4(inputPosition, 1.0);\n\
                textureCoordinates = inputTextureCoordinates;\n\
                vertexColor = inputVertexColor;\n\
            }\n\
        ";

    const char *vertexShaderSourceNone = "\n\
            #version 330 core\n\
            layout (location = 0) in vec3 inputPosition;\n\
            \n\
            out vec2 textureCoordinates;\n\
            out vec4 vertexColor;\n\
            \n\
            void main() {\n\
                gl_Position = vec4(inputPosition, 1.0);\n\
                vertexColor = vec4(1.0, 0.0, 1.0, 1.0);\n\
            }\n\
        ";

    const char *fragmentShaderSource = "\n\
            #version 330 core\n\
            out vec4 fragColor;\n\
            \n\
            uniform int renderMode;\n\
            \n\
            in vec2 textureCoordinates;\n\
            in vec4 vertexColor;\n\
            \n\
            uniform sampler2D textureSampler;\n\
            \n\
            void main() {\n\
                switch(renderMode) {\n\
                    case 0:\n\
                        fragColor = texture(textureSampler, textureCoordinates);\n\
            \n\
                    case 1:\n\
                        fragColor = vertexColor;\n\
            \n\
                    case 2:\n\
                        fragColor = mix(vertexColor, texture(textureSampler, textureCoordinates), 0.5);\n\
            \n\
                    case 3:\n\
                        fragColor = vertexColor;\n\
                }\n\
            }\n\
        ";

    int success;
    char infoLog[512];
    char failMessage[768];

    unsigned int vertexShaderTextureOnly = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShaderTextureOnly, 1, &vertexShaderSourceTextureOnly, NULL);
    context->CompileShader(vertexShaderTextureOnly);

    context->GetShaderiv(vertexShaderTextureOnly, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShaderTextureOnly, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Vertex shader 0 failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShaderTextureOnly);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    unsigned int vertexShaderColorOnly = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShaderColorOnly, 1, &vertexShaderSourceColorOnly, NULL);
    context->CompileShader(vertexShaderColorOnly);

    context->GetShaderiv(vertexShaderColorOnly, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShaderColorOnly, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Vertex shader 1 failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShaderTextureOnly);
        context->DeleteShader(vertexShaderColorOnly);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    unsigned int vertexShaderTextureAndColor = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShaderTextureAndColor, 1, &vertexShaderSourceTextureAndColor, NULL);
    context->CompileShader(vertexShaderTextureAndColor);

    context->GetShaderiv(vertexShaderTextureAndColor, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShaderTextureAndColor, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Vertex shader 2 failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShaderTextureOnly);
        context->DeleteShader(vertexShaderColorOnly);
        context->DeleteShader(vertexShaderTextureAndColor);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    unsigned int vertexShaderNone = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShaderNone, 1, &vertexShaderSourceNone, NULL);
    context->CompileShader(vertexShaderNone);

    context->GetShaderiv(vertexShaderNone, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShaderNone, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Vertex shader 3 failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShaderTextureOnly);
        context->DeleteShader(vertexShaderColorOnly);
        context->DeleteShader(vertexShaderTextureAndColor);
        context->DeleteShader(vertexShaderNone);
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
        context->DeleteShader(vertexShaderTextureOnly);
        context->DeleteShader(fragmentShader);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    unsigned int shaderProgram = context->CreateProgram();
    context->AttachShader(shaderProgram, vertexShaderTextureOnly);
    context->AttachShader(shaderProgram, fragmentShader);
    context->LinkProgram(shaderProgram);

    context->GetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        context->GetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        snprintf(failMessage, 768, "Shader program failed to link. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShaderTextureOnly);
        context->DeleteShader(fragmentShader);
        context->DeleteProgram(shaderProgram);
        prError(PR_GL_ERROR, failMessage);
        return 0;
    }

    return shaderProgram;
}
