#include "ShaderProgram.hpp"
#include <exception>
#include <iostream>
#include <vector>
#include <glad/gl.h>

using namespace std;

ShaderProgramException::ShaderProgramException(ErrorCode err)
	: _errorCode(err) {}

const char *ShaderProgramException::what() const noexcept {
	switch (_errorCode) {
		// case SHADER_FAILED: return "Shader error";
		case LINKING_FAILED: return "Linking shader program failed";
		default: return "An unknown error occured during shader program creating";
	}
}


ShaderProgram::ShaderProgram()
{
	Shader vertShader("../src/shaderSources/default.vert", GL_VERTEX_SHADER);
	Shader fragShader("../src/shaderSources/default.frag", GL_FRAGMENT_SHADER);

	cout << "Creating shader program..." << endl;

	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertShader.getShader());
	glAttachShader(_shaderProgram, fragShader.getShader());
	glLinkProgram(_shaderProgram);

	GLint isLinked = 0;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		// the maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(_shaderProgram, maxLength, &maxLength, &infoLog[0]);

		// delete the shader program if linking failed
		glDeleteProgram(_shaderProgram);
		throw ShaderProgramException(ShaderProgramException::LINKING_FAILED);
	}

	vertShader.deleteShader();
	fragShader.deleteShader();
}

GLuint ShaderProgram::getShaderProgram() const {
	return _shaderProgram;
}

void ShaderProgram::deleteShaderProgram() {
	glDeleteProgram(_shaderProgram);
	cout << "Shader program was deleted" << endl;
}
