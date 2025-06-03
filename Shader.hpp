#pragma once

#include <glad/gl.h>

// enum ShaderType {
// 	GL_VERTEX_SHADER,
// 	GL_FRAGMENT_SHADER,
// }

class Shader {
public:
	explicit Shader(const char *shaderSource, GLenum shaderType);

	GLuint getShader() const;
	void deleteShader();

private:
	const char *_shaderSource;
	GLenum _shaderType;
	GLuint _shader;

	Shader();
};

// wrong shader type
// check shader source
/*
void GetShaderiv( uint shader, enum pname, int *params );
If pname is COMPILE_STATUS, TRUE is returned if the shader was last com-
piled successfully, and FALSE is returned otherwise.
*/