#pragma once

#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../openglBuffers/VAO.hpp"
#include "../openglBuffers/VBO.hpp"
#include "../openglBuffers/EBO.hpp"
#include "../shaders/ShaderProgram.hpp"

class RenderException : public std::exception {
public:
	enum ErrorCode {
		ERR,
	};

	explicit RenderException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};

class Render {
public:
	explicit Render(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, ShaderProgram &shaderProgram);

	VAO getVAO() const;
	void glSettings();
	void cleanUp();

private:
	VAO _vao;
	EBO _ebo;
	VBO _vbo;
	ShaderProgram _shaderProgram;

	Render();
};
