#include "Renderer.hpp"
#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "ShaderProgram.hpp"

using namespace std;

RendererException::RendererException(ErrorCode err)
	: _errorCode(err) {}

const char *RendererException::what() const noexcept {
	switch (_errorCode) {
		case ERR: return "Renderer error";
		default: return "An unknown error occured during shader creating";
	}
}

Renderer::Renderer(vector<GLfloat> &vertices, vector<GLuint> &indices, ShaderProgram &shaderProgram)
	: _ebo(EBO(indices)), _vbo(VBO(vertices)), _shaderProgram(shaderProgram)
{
	_vao.linkAttrib();
	_vao.unbind();

	// Sets the color that will be used when clearing the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glSettings();
}

// getters //

VAO Renderer::getVAO() const {
	return _vao;
}

// other //

void Renderer::glSettings() {
	// the depth value for every pixel
	// when enabled: performing the depth test
	glEnable(GL_DEPTH_TEST);

	// glEnable(GL_CULL_FACE);
	// glFrontFace(GL_CW);
	// glCullFace(GL_BACK);
}

void Renderer::cleanUp() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	_vao.deleteVAO();
	_ebo.deleteEBO();
	_vbo.deleteVBO();
	_shaderProgram.deleteShaderProgram();
}
