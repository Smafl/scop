#include "Shader.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

// struct ShaderTypes {
// 	string name;
// 	ShaderType type;
// };

// static ShaderType shaderTypeToEnum(const string &shaderType) {
// 	ShaderTypes shaderTypes[] = {
// 		{"vertex", GL_VERTEX_SHADER},
// 		{"fragment", GL_FRAGMENT_SHADER},
// 	}

// 	for (const auto type : shaderTypes) {
// 		if (shaderType == type.name) {
// 			return type.type;
// 		}
// 	}
// }

Shader::Shader(const char *shaderSource, GLenum shaderType)
	: _shaderSource(shaderSource), _shaderType(shaderType)
{
	_shader = glCreateShader(_shaderType);
	glShaderSource(_shader, 1, &_shaderSource, NULL);
	glCompileShader(_shader);
}

GLuint Shader::getShader() const {
	return _shader;
}

void Shader::deleteShader() {
	glDeleteShader(_shader);
}
