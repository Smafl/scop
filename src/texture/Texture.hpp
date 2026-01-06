#pragma once

#include "BMPLoader.hpp"
#include <exception>
#include <glad/gl.h>
#include <string>
#include <vector>

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

struct ImageData {
	uint32_t width;
    uint32_t height;
	GLenum formatBMPImage;
	GLuint texture;
	const unsigned char * _pixelData; // BGR
};

class Texture {
public:
	explicit Texture(const std::string &path);

	// void bindTexture();
	void cleanUp();

	const ImageData &getImageData() const;

private:
	BMPLoader _bmpImage;
	std::string _path;
	ImageData _imageData;

	void createTexture();
	Texture();
};
