#pragma once

#include <glad/gl.h>
#include <vector>

// Vertex buffer object
class VBO {
public:

	explicit VBO(std::vector<GLfloat> vertices) {
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	void deleteVBO() {
		glDeleteBuffers(1, &_vbo);
	}

	GLuint getVBO() const {
		return _vbo;
	}

private:
	GLuint _vbo;

	VBO();
};
