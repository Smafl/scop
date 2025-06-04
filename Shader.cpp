#include "Shader.hpp"
#include <iostream>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

ShaderException::ShaderException(ErrorCode err)
	: _errorCode(err) {}

const char *ShaderException::what() const noexcept {
	switch (_errorCode) {
		case TYPE_NOT_FOUND: return "Not existing shader type";
		case SOURCE_NOT_FOUND: return "Source for shader not found";
		case COMPILATION_FAILED: return "Shader compilation failed";
		default: return "An unknown error occured during shader creating";
	}
}

Shader::Shader(const char *shaderSource, GLenum shaderType)
	: _shaderSource(shaderSource), _shaderType(shaderType)
{
	if (!isValidShaderType(_shaderType)) {
		throw ShaderException(ShaderException::TYPE_NOT_FOUND);
	}

	if (shaderSource == nullptr || shaderSource[0] == '\0') {
		throw ShaderException(ShaderException::SOURCE_NOT_FOUND);
	}

	_shader = glCreateShader(_shaderType);
	glShaderSource(_shader, 1, &_shaderSource, NULL);
	glCompileShader(_shader);

	GLint isCompiled = 0;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &maxLength);

		// the maxLength includes the NULL character
		vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(_shader, maxLength, &maxLength, &errorLog[0]);
		cerr << "Shader compilation failed: " << errorLog.data();

		// delete the shader if compilation failed
		glDeleteShader(_shader);
		throw ShaderException(ShaderException::COMPILATION_FAILED);
	}
}

const GLenum Shader::_shaderTypes[5] = {
	GL_VERTEX_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_GEOMETRY_SHADER,
	GL_FRAGMENT_SHADER,
};

bool Shader::isValidShaderType(GLenum type) const {
	for (int i = 0; i < 5; ++i) {
		if (_shaderTypes[i] == type)
			return true;
	}
	return false;
}

GLuint Shader::getShader() const {
	return _shader;
}

void Shader::deleteShader() {
	glDeleteShader(_shader);
}
