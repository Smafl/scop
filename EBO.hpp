#pragma once

#include <glad/gl.h>
#include <vector>

// Vertex array object
class EBO {
public:
	GLuint _ebo; // move in private

	explicit EBO(std::vector<GLuint> indices) {
		glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}

	void deleteEBO() {
		glDeleteBuffers(1, &_ebo);
	}
	// GLuint getEBO() const;

private:
	EBO();
};
