#include "Render.hpp"
#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../shaders/ShaderProgram.hpp"
#include "../modelLoader/RenderModelLoader.hpp"

using namespace std;

RenderException::RenderException(ErrorCode err)
	: _errorCode(err) {}

const char *RenderException::what() const noexcept {
	switch (_errorCode) {
		case ERR: return "Render error";
		default: return "An unknown error occured during shader creating";
	}
}

Render::Render(Mesh &mesh, ShaderProgram &shaderProgram)
	: _shaderProgram(shaderProgram), _mesh(mesh)
{

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO_pos);
    glGenBuffers(1, &_VBO_tex);
    glGenBuffers(1, &_VBO_norm);
    glGenBuffers(1, &_EBO);


	// Bind VAO
	glBindVertexArray(_VAO);

	// Position buffer (location 0)
	glBindBuffer(GL_ARRAY_BUFFER, _VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, _mesh.vertices.size() * sizeof(float),
					_mesh.vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate buffer (location 1)
	glBindBuffer(GL_ARRAY_BUFFER, _VBO_tex);
	glBufferData(GL_ARRAY_BUFFER, _mesh.texCoords.size() * sizeof(float),
					_mesh.texCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// Normal buffer (location 2)
	glBindBuffer(GL_ARRAY_BUFFER, _VBO_norm);
	glBufferData(GL_ARRAY_BUFFER, _mesh.normals.size() * sizeof(float),
					_mesh.normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh.indices.size() * sizeof(unsigned int),
					_mesh.indices.data(), GL_STATIC_DRAW);


	// Unbind VAO
	glBindVertexArray(0);

	// Sets the color that will be used when clearing the screen
	// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glSettings();
}

// getters //

GLuint Render::getVAO() const {
	return _VAO;
}

// other //

void Render::glSettings() {
	// the depth value for every pixel
	// when enabled: performing the depth test
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// glEnable(GL_CULL_FACE);
	// glFrontFace(GL_CW);
	// glCullFace(GL_BACK);
}

void Render::cleanUp() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO_pos);
    glDeleteBuffers(1, &_VBO_tex);
    glDeleteBuffers(1, &_VBO_norm);
    glDeleteBuffers(1, &_EBO);
	_shaderProgram.deleteShaderProgram();
}
