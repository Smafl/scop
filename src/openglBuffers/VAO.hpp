#pragma once

#include <glad/gl.h>

// Vertex array object
class VAO {
public:
	VAO() {
		glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
	}

	void linkAttrib() {
        // Position attribute
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

		// Texture coord attribute (location = 1)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the object
        glBindVertexArray(0);
	}

	void deleteVAO() {
		glDeleteVertexArrays(1, &_vao);
	}

	GLuint getVAO() const {
		return _vao;
	}

private:
	GLuint _vao;
};
