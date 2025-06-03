#include <iostream>
#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

using namespace std;

int main(void) {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit()) {
        cerr << "Cannot initialize GLFW" << endl;
        return -1;
    }

    // Use version 4.1 (last available on MacOS)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWindow object and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        cerr << "GLFW cannot create a window" << endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Model model;
    const vector<GLfloat> vertices = model.getVertices();

    // Vertex Shader source
    const char* vertexShaderSource = R"(
        #version 410 core
        layout (location = 0) in vec2 aPos;
        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
        })";

    // Fragment Shader source
    const char* fragmentShaderSource = R"(
        #version 410 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.2, 0.3, 1.0);
        })";

    // Check OpenGL version
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "OpenGL version: " << version << endl;

    vector<GLuint> shaders;
    Shader *vertexShaderInstance = new Shader(vertexShaderSource, GL_VERTEX_SHADER);
    shaders.push_back(vertexShaderInstance->getShader());

    Shader *fragmentShaderInstance = new Shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    shaders.push_back(fragmentShaderInstance->getShader());

    ShaderProgram *shaderProgramInstance = new ShaderProgram(shaders);
    GLuint shaderProgram = shaderProgramInstance->getShaderProgram();

    vertexShaderInstance->deleteShader();
    fragmentShaderInstance->deleteShader();

    // Vertex array and vertex buffer object (order matters)
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the object
    glBindVertexArray(0);

    // Sets the color that will be used when clearing the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here

        // Crears the screen
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    delete vertexShaderInstance;
    delete fragmentShaderInstance;
    delete shaderProgramInstance;

    return 0;
}
