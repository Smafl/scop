#include <iostream>
#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Window.hpp"
#include "RenderModel.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Renderer.hpp"
#include "MatrixTransform.hpp"
#include "InputListener.hpp"

using namespace std;

double prevTime = glfwGetTime();

// void parseArgv(char** argv, int *width, int *height, char *windowName) {
//     ;
// }

void getHelp() {
    cout << "File with a model is mandatory after executable file name" << endl;
    cout << "Optional arguments: window width, height and name" << endl;
    cout << endl;
    cout << "Example 1: ./scop models/zombie.obj" << endl;
    cout << "Example 2: ./scop models/zombie.obj 640 480" << endl;
    cout << "Example 3: ./scop models/zombie.obj 640 480 Zombie" << endl;
}

int main(int args, char* argv[]) {

    int width = 0, height = 0;
    // char *objFile = argv[1];
    char *windowName = nullptr;

    if (args < 2) {
        cerr << "No object file given. Run './scop help' to get help" << endl;
        return EXIT_FAILURE;
    } else if (static_cast<string>(argv[1]) == "help") {
        getHelp();
        return 0;
    }
    // } else {
    //     parseArgv(argv, &width, &height, windowName);
    // }

    try {
        RenderModel renderModel(argv[1]);
        vector<GLfloat> vertices = renderModel.getVertices();
        vector<GLuint> indices = renderModel.getVIndices();

        Window windowInstance(width, height, windowName);
        GLFWwindow *window = windowInstance.getWindow();

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            throw runtime_error("Failed to initialize GLAD");
        }

        // // Check OpenGL version
        // const GLubyte* version = glGetString(GL_VERSION);
        // cout << "OpenGL version: " << version << endl;

        vector<Shader> shaders;
        Shader vertexShaderInstance("../shaders/default.vert", GL_VERTEX_SHADER);
        shaders.push_back(vertexShaderInstance);

        Shader fragmentShaderInstance("../shaders/default.frag", GL_FRAGMENT_SHADER);
        shaders.push_back(fragmentShaderInstance);

        ShaderProgram shaderProgram(shaders);

        // can be moved in destructor if no more shader manipulations
        vertexShaderInstance.deleteShader();
        fragmentShaderInstance.deleteShader();

        Renderer renderer(vertices, indices, shaderProgram);

        // the field of view
        GLfloat fov = 45.0f;
        GLfloat aspect = (GLfloat)windowInstance.getScreenWidth() / (GLfloat)windowInstance.getScreenHeight();
        GLfloat near = 0.1f;
        GLfloat far = 100.0f;

        // Set user pointer for callback access
        glfwSetWindowUserPointer(window, &renderModel);

	    // Set input callback
	    glfwSetKeyCallback(window, InputListener::key_callback);
	    glfwSetMouseButtonCallback(window, InputListener::mouse_button_callback);
	    glfwSetScrollCallback(window, InputListener::scroll_callback);

        // Loop until the user closes the window
        while (!windowInstance.shouldCloseWindow()) {
            // Render here

            // Crears the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram.getShaderProgram());

            if (renderModel.isRotate) {
                double currentTime = glfwGetTime();
                if (currentTime - prevTime >= 1 / 60) {
                    renderModel.rotation += 0.25f;
                    prevTime = currentTime;
                }
            }

            GLfloat modelMatrix[16], projectionMatrix[16];
            GLfloat scale[16], rotate[16];

            MatrixTransform::loadIdentity(scale);
            MatrixTransform::scale(scale, renderModel.scaleFactor);

            MatrixTransform::loadIdentity(rotate);
            MatrixTransform::rotateY(rotate, renderModel.rotation);
            MatrixTransform::multiply(rotate, scale, modelMatrix);

            MatrixTransform::translate(modelMatrix, renderModel.translationX, renderModel.translationY, renderModel.translationZ);

            MatrixTransform::loadIdentity(projectionMatrix);
            MatrixTransform::perspective(projectionMatrix, fov, aspect, near, far);

            int modelMatrixLoc = glGetUniformLocation(shaderProgram.getShaderProgram(), "modelMatrix");
            if (modelMatrixLoc == -1) {
                throw runtime_error("Error getting uniform location of model matrix");
            }
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_TRUE, modelMatrix);

            // int viewMatrixLoc = glGetUniformLocation(shaderProgram.getShaderProgram(), "viewMatrix");
            // glUniformMatrix4fv(viewMatrixLoc, 1, GL_TRUE, viewMatrix);
            int projectionMatrixLoc = glGetUniformLocation(shaderProgram.getShaderProgram(), "projectionMatrix");
            if (projectionMatrixLoc == -1) {
                throw runtime_error("Error getting uniform location of projection matrix");
            }
            glUniformMatrix4fv(projectionMatrixLoc, 1, GL_TRUE, projectionMatrix);

            glBindVertexArray(renderer.getVAO().getVAO());
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

            // Swap front and back buffers
            windowInstance.swapBuffers();

            // Poll for and process events
            glfwPollEvents();
        }

        renderer.cleanUp();
        return 0;

    } catch (const RenderModelException &e) {
        cerr << "Rendering model error: " << e.what() << endl;
    } catch (const ShaderException &e) {
        cerr << "Shader error: " << e.what() << endl;
    } catch (const ShaderProgramException &e) {
        cerr << "Shader program error: " << e.what() << endl;
    } catch (const MatrixTransformException &e) {
        cerr << "Matrix transformation error: " << e.what() << endl;
    } catch (const exception &e) {
        cerr << e.what() << endl;
    } catch (...) {
        cerr << "An unknown exception occured" << endl;
    }
    return EXIT_FAILURE;
}
