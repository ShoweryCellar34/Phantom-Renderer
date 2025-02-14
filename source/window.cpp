#include <PR/window.hpp>

#include <iostream>
#include <PR/mesh.hpp>

void PR::window::makeWindow(std::string title, int width, int height) {
    i_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}

void PR::window::makeContext() {
    glfwMakeContextCurrent(i_window);
    gladLoadGLContext(&i_openglContext, (GLADloadfunc)glfwGetProcAddress);
}

void PR::window::prepMesh(meshData mesh, std::string alias) {
    unsigned int VBO, VAO, EBO;
    i_openglContext.GenVertexArrays(1, &VAO);
    i_openglContext.GenBuffers(1, &VBO);
    i_openglContext.GenBuffers(1, &EBO);
    i_openglContext.BindVertexArray(VAO);

    i_openglContext.BindBuffer(GL_ARRAY_BUFFER, VBO);
    i_openglContext.BufferData(GL_ARRAY_BUFFER, mesh.i_verticesCount, mesh.i_vertices, GL_STATIC_DRAW);

    i_openglContext.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    i_openglContext.BufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.i_indicesCount, mesh.i_indices, GL_STATIC_DRAW);

    i_openglContext.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    i_openglContext.EnableVertexAttribArray(0);

    i_openglContext.BindBuffer(GL_ARRAY_BUFFER, 0); 

    i_openglContext.BindVertexArray(0);

    i_VAOList.insert({alias, {VAO, mesh.i_indicesCount}});
}

void PR::window::drawMesh(std::string alias) {
    i_openglContext.BindVertexArray(i_VAOList.at(alias).first);
    i_openglContext.DrawElements(GL_TRIANGLES, i_VAOList.at(alias).second, GL_UNSIGNED_INT, 0);
}

unsigned int PR::window::genDefaultShaderProgram()
{
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

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
