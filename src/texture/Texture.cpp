#include "Texture.hpp"
#include "BMPLoader.hpp"
#include <glad/gl.h>
#include <exception>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>

using namespace std;

TextureException::TextureException(ErrorCode err)
	: _errorCode(err) {}

const char *TextureException::what() const noexcept {
	switch (_errorCode) {
		case NO_DATA: return "No texture data";
		default: return "An unknown error occured during texture creating";
	}
}

Texture::Texture(const string &path) :
	_bmpImage(path)
{
    // std::cout << "Loading texture: " << path << std::endl;
    // std::cout << "  Width: " << _bmpImage.getBMPHeader()->width << std::endl;
    // std::cout << "  Height: " << _bmpImage.getBMPHeader()->height << std::endl;
    // std::cout << "  Channels: " << _bmpImage.channels << std::endl;
    // std::cout << "  Bits per pixel: " << _bmpImage.getBMPHeader()->bitsPerPixel << std::endl;

    // // Check first few pixels
    // const unsigned char* data = _bmpImage.getPixelData();
    // std::cout << "  First pixel RGB: "
    //           << (int)data[0] << ", "
    //           << (int)data[1] << ", "
    //           << (int)data[2] << std::endl;

	glGenTextures(1, &_textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	GLenum formatBMPImage =(_bmpImage.channels == 4) ? GL_BGRA : GL_BGR;
	GLenum internalFormat = (_bmpImage.channels == 4) ? GL_RGBA : GL_RGB;

	glTexImage2D(
		GL_TEXTURE_2D, 0,
		internalFormat,
		_bmpImage.getBMPHeader()->width,
		_bmpImage.getBMPHeader()->height,
		0, formatBMPImage, GL_UNSIGNED_BYTE,
		_bmpImage.getPixelData()
	);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // Not REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // Change from NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

GLuint Texture::getTextureID() const {
	return _textureID;
}

void Texture::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
