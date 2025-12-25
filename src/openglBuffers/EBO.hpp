#pragma once

#include <glad/gl.h>
#include <vector>

// Element array buffer object
class EBO {
public:
	explicit EBO(std::vector<GLuint> indices) {
		glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	}

	void deleteEBO() {
		glDeleteBuffers(1, &_ebo);
	}

	GLuint getEBO() const {
		return _ebo;
	}

private:
	GLuint _ebo;

	EBO();
};
