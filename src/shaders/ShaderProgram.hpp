#pragma once

#include <vector>
#include <exception>
#include <glad/gl.h>
#include "Shader.hpp"

class ShaderProgramException : public std::exception {
public:
	enum ErrorCode {
		SHADERS_NOT_FOUND,
		LINKING_FAILED,
	};

	explicit ShaderProgramException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};

class ShaderProgram {
public:
	explicit ShaderProgram(std::vector<Shader> &shaders);
	GLuint getShaderProgram() const;
	void deleteShaderProgram();

private:
	GLuint _shaderProgram;

	ShaderProgram();
};
