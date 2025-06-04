#pragma once

#include <exception>
#include <glad/gl.h>

using namespace std;

class ShaderException : public exception {
public:
	enum ErrorCode {
		TYPE_NOT_FOUND,
		SOURCE_NOT_FOUND,
		COMPILATION_FAILED,
	};

	explicit ShaderException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};

class Shader {
public:
	explicit Shader(const char *shaderSource, GLenum shaderType);

	GLuint getShader() const;
	void deleteShader();

private:
	const char *_shaderSource;
	GLenum _shaderType;
	GLuint _shader;
	static const GLuint _shaderTypes[5];

	bool isValidShaderType(GLenum type) const;
	Shader();
};
