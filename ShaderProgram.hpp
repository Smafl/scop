#pragma once

#include <vector>
#include <glad/gl.h>

using namespace std;

class ShaderProgram {
public:
	explicit ShaderProgram(vector<GLuint> &shaders);
	GLuint getShaderProgram() const;

private:
	GLuint _shaderProgram;

	ShaderProgram();
};
