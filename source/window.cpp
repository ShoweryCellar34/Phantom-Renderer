#include <PR/window.hpp>

void PR::window::makeTestTri() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };


    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

    unsigned int vertexShader;
    vertexShader = openglContext.CreateShader(GL_VERTEX_SHADER);
    openglContext.ShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    openglContext.CompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = openglContext.CreateShader(GL_FRAGMENT_SHADER);
    openglContext.ShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    openglContext.CompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = openglContext.CreateProgram();
    openglContext.AttachShader(shaderProgram, vertexShader);
    openglContext.AttachShader(shaderProgram, fragmentShader);
    openglContext.LinkProgram(shaderProgram);

    unsigned int VBO;
    openglContext.GenBuffers(1, &VBO);
    openglContext.BindBuffer(GL_ARRAY_BUFFER, VBO);
    openglContext.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    openglContext.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    openglContext.EnableVertexAttribArray(0);
    openglContext.UseProgram(shaderProgram);

    unsigned int VAO;
    openglContext.GenVertexArrays(1, &VAO);
    openglContext.BindVertexArray(VAO);
    openglContext.BindBuffer(GL_ARRAY_BUFFER, VBO);
    openglContext.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    openglContext.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    openglContext.EnableVertexAttribArray(0);

    testTriShaderProgram = shaderProgram;
    testTriVAO = VAO;
}

void PR::window::makeWindow(std::string title, int width, int height) {
    windowInternal = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
}

void PR::window::makeContext() {
    glfwMakeContextCurrent(windowInternal);
    gladLoadGLContext(&openglContext, (GLADloadfunc)glfwGetProcAddress);
}

unsigned int PR::window::prepMesh(float vertices[], unsigned int indices[]) {
    unsigned int VBO, VAO, EBO;
    openglContext.GenVertexArrays(1, &VAO);
    openglContext.GenBuffers(1, &VBO);
    openglContext.GenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    openglContext.BindVertexArray(VAO);

    openglContext.BindBuffer(GL_ARRAY_BUFFER, VBO);
    openglContext.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    openglContext.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    openglContext.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    openglContext.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    openglContext.EnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    openglContext.BindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    openglContext.BindVertexArray(0); 

    stuff.push_back({EBO, VAO});
    return stuff.size() - 1;
}
