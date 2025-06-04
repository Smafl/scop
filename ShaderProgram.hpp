#pragma once

#include <vector>
#include <glad/gl.h>

using namespace std;

class ShaderProgramException : public exception {
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
	explicit ShaderProgram(vector<GLuint> &shaders);
	GLuint getShaderProgram() const;

private:
	GLuint _shaderProgram;

	ShaderProgram();
};
