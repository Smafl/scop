#include <iostream>
#include <exception>
#include <vector>
#include <array>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // redo
#include <glm/gtc/matrix_transform.hpp> // redo
#include <glm/gtc/type_ptr.hpp> // redo
#include "Model.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

using namespace std;

// vector<GLfloat> translate(vector<GLfloat> identityMatrix, vector<GLfloat> vertices) {
//     vector<GLfloat> translatedVector;
//     for (auto v : vertices) {
//         for (int i = 0; i < 4; i++) {
//             for (auto m : identityMatrix) {
//             }
//         }
//     }
//     return translatedVector;
// }

int terminateWindow(GLFWwindow* window) {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    return EXIT_FAILURE;
}

int main(void) {
    GLFWwindow* window;
    const unsigned int width = 640;
    const unsigned int height = 480;

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
        window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
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
        Shader vertexShaderInstance("../shaders/default.vert", GL_VERTEX_SHADER);
        shaders.push_back(vertexShaderInstance.getShader());

        Shader fragmentShaderInstance("../shaders/default.frag", GL_FRAGMENT_SHADER);
        shaders.push_back(fragmentShaderInstance.getShader());

        ShaderProgram shaderProgramInstance(shaders);
        GLuint shaderProgram = shaderProgramInstance.getShaderProgram();

        vertexShaderInstance.deleteShader();
        fragmentShaderInstance.deleteShader();

        vector<GLfloat> identityMatrix = {
            1, 0, 0, 3,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };

        // vector<GLfloat> translatedVector = translate(identityMatrix, vertices);
        // for (auto i : translatedVector) {
        //     cout << i << " ";
        // }

        GLuint indices[] = {
            0,1,2,
            0,2,3,
            0,1,4,
            1,2,4,
            2,3,4,
            3,0,4,
        };

        // Vertex array and vertex buffer object (order matters)
        GLuint VAO, VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the object
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Sets the color that will be used when clearing the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        float rotation = 0.0f;
        double prevTime = glfwGetTime();

        glEnable(GL_DEPTH_TEST);

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window)) {
            // Render here

            // Crears the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram);

            double currentTime = glfwGetTime();
            if (currentTime - prevTime >= 1 / 60) {
                rotation += 0.5f;
                prevTime = currentTime;
            }

            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 proj = glm::mat4(1.0f);

            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
            proj = glm::perspective(glm::radians(45.0f), (float)(width/height), 0.1f, 100.0f);

            int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            int viewLoc = glGetUniformLocation(shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            int projLoc = glGetUniformLocation(shaderProgram, "proj");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

            glBindVertexArray(VAO);
            // glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

            // Swap front and back buffers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        // glDeleteBuffers(1, &EBO);
        glDeleteProgram(shaderProgram);

        glfwDestroyWindow(window);
        glfwTerminate();

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
