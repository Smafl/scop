#include <iostream>
#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Model.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

using namespace std;

int terminateWindow(GLFWwindow* window) {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    return EXIT_FAILURE;
}

int main(void) {
    GLFWwindow* window;

    try {
        // Initialize the library
        if (!glfwInit()) {
            throw runtime_error("Cannot initialize GLFW");
        }

        // Use version 4.1 (last available on MacOS)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Create a GLFWindow object and its OpenGL context
        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window) {
            glfwTerminate();
            throw runtime_error("GLFW cannot create a window");
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            throw runtime_error("Failed to initialize GLAD");
        }

        Model model;
        const vector<GLfloat> vertices = model.getVertices();
        if (vertices.empty()) {
            throw runtime_error("Vertices is empty");
        }

        // Check OpenGL version
        const GLubyte* version = glGetString(GL_VERSION);
        cout << "OpenGL version: " << version << endl;

        vector<GLuint> shaders;
        Shader *vertexShaderInstance = new Shader("../shaders/default.vert", GL_VERTEX_SHADER);
        shaders.push_back(vertexShaderInstance->getShader());

        Shader *fragmentShaderInstance = new Shader("../shaders/default.frag", GL_FRAGMENT_SHADER);
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

    } catch (const ShaderException &e) {
        cerr << "Shader error: " << e.what() << endl;
    } catch (const ShaderProgramException &e) {
        cerr << "Shader program error: " << e.what() << endl;
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return terminateWindow(window);
    } catch (...) {
        cerr << "An unknown exception occured" << endl;
        return terminateWindow(window);
    }
}
