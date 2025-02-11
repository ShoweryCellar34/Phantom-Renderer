#include <PR/PR.hpp>

float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
   -0.5f, -0.5f, 0.0f,  // bottom left
   -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
   0, 1, 3,  // first Triangle
   1, 2, 3   // second Triangle
};

int main(int argc, char** argv) {
    glfwInit();

    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    PR::window test;
    test.makeWindow("Test", 600, 600);
    test.makeContext();
    unsigned int shaderProgram = test.genDefaultSHaderProgram();
    unsigned int data = test.prepMesh(vertices, indices);

    int width, height;
    glfwGetWindowSize(test.windowInternal, &width, &height);
    test.openglContext.Viewport(0, 0, width, height);

    while(!glfwWindowShouldClose(test.windowInternal)) {
        // test.openglContext.ClearColor(0.5f, 0.1f, 0.9f, 0.9f);
        test.openglContext.Clear(GL_COLOR_BUFFER_BIT);

        // Render
        test.openglContext.UseProgram(shaderProgram);
        test.openglContext.BindVertexArray(1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        test.openglContext.DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(test.windowInternal);

        glfwPollEvents();
    }
}
