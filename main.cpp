#include <iostream>
#include <exception>
#include <vector>
#include <array>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "RenderModel.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "MatrixTransform.hpp"
#include <math.h>
#include <iomanip>  // For std::setw and formatting

using namespace std;

// void perspective(GLfloat *matrix, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far, GLfloat *result) {
//     memcpy(result, matrix, sizeof(GLfloat) * 16);
//     GLfloat f = 1.0f / tan(degreesToRadians(fov) / 2.0f);

//     result[0] = f / aspect;
//     result[5] = f;
//     result[10] = (far+near)/(near-far);
//     result[11] = -1;
//     result[14] = (2*far*near)/(near-far);
// }

void translation(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z, GLfloat *result) {
    memcpy(result, matrix, sizeof(GLfloat) * 16);
    result[3] = matrix[3] + x;
    result[7] = matrix[7] + y;
    result[11] = matrix[11] + z;
}

void printMatrix(GLfloat *matrix) {
    // cout << fixed << setprecision(4);
    int i = 0;
    for (int iter = 0; iter < 16; iter++) {
        cout << matrix[iter] << "\t";
        i++;
        if (i == 4) {
            cout << endl;
            i = 0;
        }
    }
    cout << endl;
}

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

        RenderModel renderModel;
        const vector<GLfloat> vertices = renderModel.getVertices();
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

        GLuint indices[] = {
            // pyramid
            0,1,2,
            0,2,3,
            0,1,4,
            1,2,4,
            2,3,4,
            3,0,4,

            // cube
            // 0, 1, 2,
            // 2, 3, 0,
            // 1, 5, 6,
            // 6, 2, 1,
            // 5, 4, 7,
            // 7, 6, 5,
            // 4, 0, 3,
            // 3, 7, 4,
            // 3, 2, 6,
            // 6, 7, 3,
            // 4, 5, 1,
            // 1, 0, 4
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

        GLfloat scaleFactor = 0.5f;
        GLfloat rotation = 0.0f;
        double prevTime = glfwGetTime();

        // GLfloat translate = 0.0f;
        // GLfloat delta = 0.0025f;

        // GLfloat fov = 80.0f;
        // GLfloat aspect = (float)width / height;
        // GLfloat near = 0.1f;
        // GLfloat far = 100.0f;

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

            // translate += delta;
            // if (translate >= 0.5f || translate <= -0.5f) {
            //     delta *= -1.0f;
            // }

            GLfloat scale[16], rotate[16], modelMatrix[16];
            MatrixTransform::loadIdentity(scale);
            MatrixTransform::loadIdentity(rotate);
            MatrixTransform::scale(scale, scaleFactor);
            MatrixTransform::rotateX(rotate, rotation);
            MatrixTransform::multiply(scale, rotate, modelMatrix);

            // GLfloat viewMatrix[16];
            // translation(translationMatrix, translate * 2, translate, 0.0, viewMatrix);

            // GLfloat perspectiveMatrix[16];
            // perspective(perspectiveIdenMatrix, fov, aspect, near, far, perspectiveMatrix);

            // int printMatrixCounter = 0;
            // if (printMatrixCounter == 0) {
            //     printMatrix(viewMatrix);
            //     printMatrixCounter = 1;
            // }

            int modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix"); // check for error (return -1)
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix);
            // int viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
            // glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMatrix);
            // int perspectiveMatrixLoc = glGetUniformLocation(shaderProgram, "perspectiveMatrix");
            // glUniformMatrix4fv(perspectiveMatrixLoc, 1, GL_TRUE, perspectiveMatrix);

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
        glDeleteBuffers(1, &EBO);
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
