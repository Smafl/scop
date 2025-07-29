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
	Window();
	Window(const char *name);
	Window(const int width, const int height, const char *name);

	GLFWwindow* getWindow() const;
	int getScreenWidth() const;
	int getScreenHeight() const;
	void terminateWindow();

private:
	GLFWwindow* _window = nullptr;
	int _screenWidth = 0;
	int _screenHeight = 0;
	const char * _windowName = nullptr;

	void windowInit();
	void getResolution();
};
