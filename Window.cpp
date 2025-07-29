#include "Window.hpp"
#include <string>
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

using namespace std;

WindowException::WindowException(ErrorCode err)
	: _errorCode(err) {}

const char *WindowException::what() const noexcept {
	switch (_errorCode) {
		case INVALID_RESOLUTION: return "Invalid screen resolution";
		case CANNOT_INITIALIZE_GLFW: return "Cannot initialize GLFW";
		case CANNOT_CREATE_WINDOW: return "GLFW cannot create a window";
		default: return "An unknown error occured during shader creating";
	}
}

Window::Window() {
	// Initialize the library
	if (!glfwInit()) {
		throw WindowException(WindowException::CANNOT_INITIALIZE_GLFW);
	}

	getResolution();
	_screenWidth /= 2;
	_screenHeight /= 2;
	windowInit();
}

Window::Window(const char *name)
	: _windowName(name)
{
	// Initialize the library
	if (!glfwInit()) {
		throw WindowException(WindowException::CANNOT_INITIALIZE_GLFW);
	}

	getResolution();
	_screenWidth /= 2;
	_screenHeight /= 2;
	windowInit();
}

Window::Window(const int width, const int height, const char *name)
	: _screenWidth(width), _screenHeight(height), _windowName(name)
{
	// Initialize the library
	if (!glfwInit()) {
		throw WindowException(WindowException::CANNOT_INITIALIZE_GLFW);
	}
	windowInit();
}

GLFWwindow *Window::getWindow() const {
	return _window;
}

int Window::getScreenWidth() const {
	return _screenWidth;
}

int Window::getScreenHeight() const {
	return _screenHeight;
}

void Window::terminateWindow() {
    if (_window) {
        glfwDestroyWindow(_window);
    }
    glfwTerminate();
	cout << "Window was terminated" << endl;
}

// private //

void Window::windowInit() {
	cout << "Creating a window..." << endl;
	if (_windowName == nullptr || _windowName[0] == '\0') {
		_windowName = "Hello triangle";
		cout << "Default window name applied" << endl;
	}

	if (_screenWidth <= 0 || _screenHeight <= 0) {
		throw WindowException(WindowException::INVALID_RESOLUTION);
	}

	// Use version 4.1 (last available on MacOS)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a GLFWindow object and its OpenGL context
	_window = glfwCreateWindow(_screenWidth, _screenHeight, _windowName, NULL, NULL);
	if (!_window) {
		glfwTerminate();
		throw WindowException(WindowException::CANNOT_CREATE_WINDOW);
	}

	// Make the window's context current
	glfwMakeContextCurrent(_window);
}

void Window::getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	_screenWidth = mode->width;
	_screenHeight = mode->height;
}
