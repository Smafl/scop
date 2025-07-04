#include <iostream>
#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "RenderModel.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "MatrixTransform.hpp"
#include <iomanip>  // For std::setw and formatting

using namespace std;

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
            // 0,1,2,
            // 0,2,3,
            // 0,1,4,
            // 1,2,4,
            // 2,3,4,
            // 3,0,4,

            // cube
            0, 1, 2,
            2, 3, 0,
            1, 5, 6,
            6, 2, 1,
            5, 4, 7,
            7, 6, 5,
            4, 0, 3,
            3, 7, 4,
            3, 2, 6,
            6, 7, 3,
            4, 5, 1,
            1, 0, 4
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

        GLfloat scaleFactor = 0.25f;
        GLfloat rotation = 0.0f;
        double prevTime = glfwGetTime();

        GLfloat translation = 0.0f;
        GLfloat delta = 0.00025f;

        // the field of view
        GLfloat fov = 45.0f;
        GLfloat aspect = (GLfloat)width / (GLfloat)height;
        GLfloat near = 0.1f;
        GLfloat far = 100.0f;

        // the depth value for every pixel
        // when enabled: performing the depth test
        glEnable(GL_DEPTH_TEST);

        // glEnable(GL_CULL_FACE);
        // glFrontFace(GL_CW);
        // glCullFace(GL_BACK);

        // Loop until the user closes the window
        // int printMatrixCounter = 0;
        while (!glfwWindowShouldClose(window)) {
            // Render here

            // Crears the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram);

            double currentTime = glfwGetTime();
            if (currentTime - prevTime >= 1 / 60) {
                rotation += 0.25f;
                prevTime = currentTime;
            }

            translation += delta;
            if (translation >= 0.25f || translation <= -0.25f) {
                delta *= -1.0f;
            }

            GLfloat modelMatrix[16], projectionMatrix[16];
            GLfloat scale[16], rotate[16];

            MatrixTransform::loadIdentity(scale);
            MatrixTransform::scale(scale, scaleFactor);

            MatrixTransform::loadIdentity(rotate);
            MatrixTransform::rotateY(rotate, rotation);
            MatrixTransform::multiply(rotate, scale, modelMatrix);

            MatrixTransform::translate(modelMatrix, translation * 2, translation, 0.0f);

            MatrixTransform::loadIdentity(projectionMatrix);
            MatrixTransform::perspective(projectionMatrix, fov, aspect, near, far);

            // if (printMatrixCounter == 0) {
            //     printMatrix(modelMatrix);
            //     printMatrixCounter = 1;
            // }

            int modelMatrixLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
            if (modelMatrixLoc == -1) {
                throw runtime_error("Error getting uniform location of model matrix");
            }
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix);

            // int viewMatrixLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
            // glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMatrix);
            int projectionMatrixLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
            if (projectionMatrixLoc == -1) {
                throw runtime_error("Error getting uniform location of projection matrix");
            }
            glUniformMatrix4fv(projectionMatrixLoc, 1, GL_TRUE, projectionMatrix);

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
    } catch (const MatrixTransformException &e) {
        cerr << "Matrix transformation error: " << e.what() << endl;
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return terminateWindow(window);
    } catch (...) {
        cerr << "An unknown exception occured" << endl;
        return terminateWindow(window);
    }
}
