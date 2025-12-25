#include "ShaderProgram.hpp"
#include "iostream"
#include <vector>
#include <glad/gl.h>

using namespace std;

ShaderProgramException::ShaderProgramException(ErrorCode err)
	: _errorCode(err) {}

const char *ShaderProgramException::what() const noexcept {
	switch (_errorCode) {
		case SHADERS_NOT_FOUND: return "Shaders not found";
		case LINKING_FAILED: return "Linking shader program failed";
		default: return "An unknown error occured during shader program creating";
	}
}

ShaderProgram::ShaderProgram(vector<Shader> &shaders)
{
	if (shaders.empty()) {
		throw ShaderProgramException(ShaderProgramException::SHADERS_NOT_FOUND);
	}

	cout << "Creating shader program..." << endl;

	_shaderProgram = glCreateProgram();
	for (const auto &shader : shaders) {
		glAttachShader(_shaderProgram, shader.getShader());
	}
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
}

GLuint ShaderProgram::getShaderProgram() const {
	return _shaderProgram;
}

void ShaderProgram::deleteShaderProgram() {
	glDeleteProgram(_shaderProgram);
	cout << "Shader program was deleted" << endl;
}
