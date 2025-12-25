#include "Render.hpp"
#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../openglBuffers/VAO.hpp"
#include "../openglBuffers/VBO.hpp"
#include "../openglBuffers/EBO.hpp"
#include "../shaders/ShaderProgram.hpp"

using namespace std;

RenderException::RenderException(ErrorCode err)
	: _errorCode(err) {}

const char *RenderException::what() const noexcept {
	switch (_errorCode) {
		case ERR: return "Render error";
		default: return "An unknown error occured during shader creating";
	}
}

Render::Render(vector<GLfloat> &vertices, vector<GLuint> &indices, ShaderProgram &shaderProgram)
	: _ebo(EBO(indices)), _vbo(VBO(vertices)), _shaderProgram(shaderProgram)
{
	_vao.linkAttrib();
	_vao.unbind();

	// Sets the color that will be used when clearing the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glSettings();
}

// getters //

VAO Render::getVAO() const {
	return _vao;
}

// other //

void Render::glSettings() {
	// the depth value for every pixel
	// when enabled: performing the depth test
	glEnable(GL_DEPTH_TEST);

	// glEnable(GL_CULL_FACE);
	// glFrontFace(GL_CW);
	// glCullFace(GL_BACK);
}

void Render::cleanUp() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	_vao.deleteVAO();
	_ebo.deleteEBO();
	_vbo.deleteVBO();
	_shaderProgram.deleteShaderProgram();
}
