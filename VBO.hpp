#pragma once

#include <glad/gl.h>
#include <vector>

// Vertex array object
class VBO {
public:
	GLuint _vbo; // move in private

	explicit VBO(std::vector<GLfloat> vertices) {
		glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}

	void deleteVBO() {
		glDeleteBuffers(1, &_vbo);
	}
	// GLuint getVBO() const;

private:
	VBO();
};
