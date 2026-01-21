#pragma once

#include "BMPLoader.hpp"
#include <exception>
#include <glad/gl.h>
#include <string>

class TextureException : public std::exception {
public:
	enum ErrorCode {
		NO_DATA,
	};

	explicit TextureException(ErrorCode err);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
};

class Texture {
public:
	explicit Texture(const std::string &path);

	GLuint getTextureID() const;
	void unbindTexture();

private:
	BMPLoader _bmpImage;
	GLuint _textureID;

	Texture();
};
