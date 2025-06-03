#include "ShaderProgram.hpp"
#include <vector>
#include <glad/gl.h>

using namespace std;

ShaderProgram::ShaderProgram(vector<GLuint> &shaders)
{
	_shaderProgram = glCreateProgram();

	// throw if shaders is empty
	for (const auto &shader : shaders) {
		glAttachShader(_shaderProgram, shader);
	}

	glLinkProgram(_shaderProgram);
}

GLuint ShaderProgram::getShaderProgram() const {
	return _shaderProgram;
}
