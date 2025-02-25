#include <PR/window.hpp>

#include <iostream>
#include <PR/mesh.hpp>
#include <PR/texture.hpp>

void PR::window::makeWindow(const std::string& title, int width, int height) {
    i_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}

void PR::window::makeContext() {
    glfwMakeContextCurrent(i_window);
    gladLoadGLContext(&i_openglContext, (GLADloadfunc)glfwGetProcAddress);
}

void PR::window::prepMesh(const meshData& mesh, const std::string& alias) {
    unsigned int VBO, VAO, EBO;
    i_openglContext.GenVertexArrays(1, &VAO);
    i_openglContext.GenBuffers(1, &VBO);
    i_openglContext.GenBuffers(1, &EBO);
    i_openglContext.BindVertexArray(VAO);

    i_openglContext.BindBuffer(GL_ARRAY_BUFFER, VBO);
    i_openglContext.BufferData(GL_ARRAY_BUFFER, mesh.i_verticesCount, mesh.i_vertices, GL_STATIC_DRAW);

    i_openglContext.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    i_openglContext.BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.i_indicesCount, mesh.i_indices, GL_STATIC_DRAW);

    i_openglContext.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    i_openglContext.EnableVertexAttribArray(0);

    i_openglContext.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    i_openglContext.EnableVertexAttribArray(0);

    i_openglContext.BindBuffer(GL_ARRAY_BUFFER, 0); 

    i_openglContext.BindVertexArray(0);

    i_VAOList.insert({alias, {VAO, mesh.i_indicesCount}});
}

void PR::window::prepTexture(const textureData& texture, const std::string& alias) {
    unsigned int TBO;
    i_openglContext.GenTextures(1, &TBO);
    i_openglContext.BindTexture(GL_TEXTURE_2D, TBO);

    i_openglContext.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    i_openglContext.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    i_openglContext.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    i_openglContext.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    i_openglContext.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.i_width, texture.i_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.i_textureData);
    i_openglContext.GenerateMipmap(GL_TEXTURE_2D);

    i_TBOList.insert({alias, TBO});
}

void PR::window::drawMesh(unsigned int shaderProgram, const std::string& meshAlias, const std::string& textureAlias) {
    i_openglContext.UseProgram(shaderProgram);

    unsigned int textureUnifromLocation = i_openglContext.GetUniformLocation(shaderProgram, "ourTexture");
    i_openglContext.Uniform1i(textureUnifromLocation, i_TBOList.at(textureAlias));

    i_openglContext.BindVertexArray(i_VAOList.at(meshAlias).first);
    i_openglContext.DrawElements(GL_TRIANGLES, i_VAOList.at(meshAlias).second, GL_UNSIGNED_INT, 0);
}

unsigned int PR::window::genDefaultShaderProgram() {
    const char *vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 2) in vec2 aTexCoord;

            out vec2 TexCoord;

            void main() {
                gl_Position = vec4(aPos, 1.0);
                TexCoord = aTexCoord;
            }
        )";

    const char *fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;

            in vec2 TexCoord;

            uniform sampler2D ourTexture;

            void main() {
                FragColor = texture(ourTexture, TexCoord);
            }
        )";

    unsigned int vertexShader = i_openglContext.CreateShader(GL_VERTEX_SHADER);
    i_openglContext.ShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    i_openglContext.CompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    i_openglContext.GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        i_openglContext.GetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = i_openglContext.CreateShader(GL_FRAGMENT_SHADER);
    i_openglContext.ShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    i_openglContext.CompileShader(fragmentShader);
    // check for shader compile errors
    i_openglContext.GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        i_openglContext.GetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = i_openglContext.CreateProgram();
    i_openglContext.AttachShader(shaderProgram, vertexShader);
    i_openglContext.AttachShader(shaderProgram, fragmentShader);
    i_openglContext.LinkProgram(shaderProgram);
    // check for linking errors
    i_openglContext.GetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        i_openglContext.GetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return shaderProgram;
}
