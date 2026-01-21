#pragma once

#include <exception>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "../shaders/ShaderProgram.hpp"
#include "../modelLoader/RenderModelLoader.hpp"

class RenderException : public std::exception {
public:
	enum ErrorCode {
		ERR,
	};

	explicit RenderException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};

class Render {
public:
	GLfloat translationX = 0.0f;
    GLfloat translationY = 0.0f;
    GLfloat translationZ = 0.0f;
    GLfloat delta = 0.05f;
    GLfloat rotation = 0.0f;
    bool isRotate = true;
    GLfloat minZ = -5.0f;
    GLfloat maxZ = 0.5f;
    GLfloat scaleFactor = 0.075f;

	// Switch texture/color
    bool textureMode = false;
	float mixValue = 0.0f;  // 0.0 = colors, 1.0 = texture
	float transitionSpeed = 1.0f;

	explicit Render(Mesh &mesh, ShaderProgram &shaderProgram);

	GLuint getVAO() const;
	void glSettings();
	void cleanUp();

private:
	GLuint _VAO, _VBO_pos, _VBO_tex, _VBO_norm, _EBO;
	ShaderProgram _shaderProgram;
	Mesh _mesh;

	Render();
};
