#pragma once

#include <glad/gl.h>

// Vertex array object
class VAO {
public:
	VAO();
	GLuint getVAO() const;

private:
	GLuint _vao;
};
