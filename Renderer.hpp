#pragma once

#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "ShaderProgram.hpp"

class RendererException : public std::exception {
public:
	enum ErrorCode {
		ERR,
	};

	explicit RendererException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};

class Renderer {
public:
	explicit Renderer(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices, ShaderProgram &shaderProgram);

	VAO getVAO() const;

	void cleanUp();

	private:
	VAO _vao;
	EBO _ebo;
	VBO _vbo;
	ShaderProgram _shaderProgram;

	Renderer();
};
