#pragma once

#include <exception>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

class WindowException : public std::exception {
public:
	enum ErrorCode {
		INVALID_RESOLUTION,
		CANNOT_INITIALIZE_GLFW,
		CANNOT_CREATE_WINDOW,
	};

	explicit WindowException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};

class Window {
public:
	Window(const int width, const int height, const char *name);

	GLFWwindow* getWindow() const;
	int getScreenWidth() const;
	int getScreenHeight() const;
	~Window();

private:
	GLFWwindow* _window = nullptr;
	int _maxWidth = 0;
	int _maxHeight = 0;
	int _width = 0;
	int _height = 0;
	const char * _windowName = nullptr;

	void getResolution();
	Window();
};
