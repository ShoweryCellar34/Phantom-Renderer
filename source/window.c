#include <PR/window.h>

#include <PR/mesh.h>
#include <PR/texture.h>
#include <stdio.h>

void prWindowInit(prWindow* window, const char* title, int width, int height) {
    window->window = glfwCreateWindow(width, height, title, NULL, NULL);
}

void prWindowInitContext(prWindow* window) {
    glfwMakeContextCurrent(window->window);
    gladLoadGLContext(&window->openglContext, (GLADloadfunc)glfwGetProcAddress);
}

void prWindowDrawMesh(prWindow* window, unsigned int shaderProgram, prMeshData* mesh, prTextureData* texture) {
    window->openglContext.UseProgram(shaderProgram);

    window->openglContext.ActiveTexture(GL_TEXTURE0);
    window->openglContext.BindTexture(GL_TEXTURE_2D, texture->TBO);

    window->openglContext.BindVertexArray(mesh->VAO);
    window->openglContext.DrawElements(GL_TRIANGLES, mesh->EBO, GL_UNSIGNED_INT, 0);
}

unsigned int prWindowGenDefaultShaderProgram(prWindow* window) {
    const char *vertexShaderSource = "\n\
            #version 330 core\n\
            layout (location = 0) in vec3 inputPosition;\n\
            layout (location = 1) in vec2 inputTextureCoordinates;\n\
            \n\
            out vec2 textureCoordinates;\n\
            \n\
            void main() {\n\
                gl_Position = vec4(inputPosition, 1.0);\n\
                textureCoordinates = inputTextureCoordinates;\n\
            }\n\
        ";

    const char *fragmentShaderSource = "\n\
            #version 330 core\n\
            out vec4 FragColor;\n\
            \n\
            in vec2 textureCoordinates;\n\
            \n\
            uniform sampler2D textureSampler;\n\
            \n\
            void main() {\n\
                FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n\
            }\n\
        ";

    unsigned int vertexShader = window->openglContext.CreateShader(GL_VERTEX_SHADER);
    window->openglContext.ShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    window->openglContext.CompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    window->openglContext.GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        window->openglContext.GetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("%s\n%s\n", "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = window->openglContext.CreateShader(GL_FRAGMENT_SHADER);
    window->openglContext.ShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    window->openglContext.CompileShader(fragmentShader);
    // check for shader compile errors
    window->openglContext.GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        window->openglContext.GetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("%s\n%s\n", "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog);
    }
    // link shaders
    unsigned int shaderProgram = window->openglContext.CreateProgram();
    window->openglContext.AttachShader(shaderProgram, vertexShader);
    window->openglContext.AttachShader(shaderProgram, fragmentShader);
    window->openglContext.LinkProgram(shaderProgram);
    // check for linking errors
    window->openglContext.GetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        window->openglContext.GetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("%s\n%s\n", "ERROR::SHADER::PROGRAM::LINKING_FAILED\n", infoLog);
    }

    return shaderProgram;
}
