#include <iostream>
#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "modelLoader/RenderModelLoader.hpp"
#include "window/Window.hpp"
#include "shaders/Shader.hpp"
#include "shaders/ShaderProgram.hpp"
#include "render/Render.hpp"
#include "matrixMath/MatrixTransform.hpp"
#include "inputHandler/InputListener.hpp"
#include "texture/Texture.hpp"

using namespace std;

double prevTime = glfwGetTime();

int main(int args, char* argv[]) {

    int width = 0, height = 0;
    char *windowName = nullptr;

    if (args < 2) {
        cout << "File with a model is mandatory after executable file name" << endl;
        return 0;
    }
    try {
        RenderModelLoader renderModel(argv[1]);
        vector<GLfloat> vertices = renderModel.getFinalVertices();
        vector<GLuint> indices = renderModel.getFinalIndices();

        Window windowInstance(width, height, windowName);
        GLFWwindow *window = windowInstance.getWindow();

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            throw runtime_error("Failed to initialize GLAD");
        }

        // Check OpenGL version
        const GLubyte* version = glGetString(GL_VERSION);
        cout << "OpenGL version: " << version << endl;

        vector<Shader> shaders;
        Shader vertexShaderInstance("../src/shaderSources/final.vert", GL_VERTEX_SHADER);
        shaders.push_back(vertexShaderInstance);

        Shader fragmentShaderInstance("../src/shaderSources/final.frag", GL_FRAGMENT_SHADER);
        shaders.push_back(fragmentShaderInstance);

        ShaderProgram shaderProgram(shaders);

        // can be moved in destructor if no more shader manipulations
        vertexShaderInstance.deleteShader();
        fragmentShaderInstance.deleteShader();

        Render render(vertices, indices, shaderProgram);

        Texture texture("../textureSources/glass.bmp");

        glUniform1i(glGetUniformLocation(shaderProgram.getShaderProgram(), "tex"), 0);

        float mixValue = 0.0f;  // 0.0 = colors, 1.0 = texture
        float transitionSpeed = 1.0f;

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
        while (!windowInstance.windowShouldClose()) {
            // Render here

            double currentTime = glfwGetTime();
            double deltaTime = currentTime - prevTime;
            prevTime = currentTime;

            // Crears the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(shaderProgram.getShaderProgram());

            if (renderModel.isRotate) {
                if (deltaTime >= 1 / 60) {
                    renderModel.rotation += 0.25f * deltaTime * 60.0f;
                }
            }

            if (renderModel.textureMode && mixValue < 1.0f) {
                mixValue += transitionSpeed * deltaTime;
                if (mixValue > 1.0f) mixValue = 1.0f;
            } else if (!renderModel.textureMode && mixValue > 0.0f) {
                mixValue -= transitionSpeed * deltaTime;
                if (mixValue < 0.0f) mixValue = 0.0f;
            }

            glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgram(), "mixValue"), mixValue);

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

            glBindVertexArray(render.getVAO().getVAO());
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

            // Swap front and back buffers
            windowInstance.swapBuffers();

            // Poll for and process events
            glfwPollEvents();
        }

        texture.unbindTexture();
        render.cleanUp();
        return 0;

    } catch (const RenderModelLoaderException &e) {
        cerr << "Rendering model error: " << e.what() << endl;
    } catch (const WindowException &e) {
        cerr << "Window error: " << e.what() << endl;
    } catch (const ShaderException &e) {
        cerr << "Shader error: " << e.what() << endl;
    } catch (const ShaderProgramException &e) {
        cerr << "Shader program error: " << e.what() << endl;
    } catch (const MatrixTransformException &e) {
        cerr << "Matrix transformation error: " << e.what() << endl;
    } catch (const BMPLoaderException &e) {
        cerr << "BMP loader error: " << e.what() << endl;
    } catch (const TextureException &e) {
        cerr << "Texture error: " << e.what() << endl;
    } catch (const exception &e) {
        cerr << e.what() << endl;
    } catch (...) {
        cerr << "An unknown exception occured" << endl;
    }
    return EXIT_FAILURE;
}
