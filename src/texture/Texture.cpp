#include "Texture.hpp"
#include "BMPLoader.hpp"
#include <glad/gl.h>
#include <exception>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

TextureException::TextureException(ErrorCode err)
	: _errorCode(err) {}

const char *TextureException::what() const noexcept {
	switch (_errorCode) {
		case NO_DATA: return "No texture data";
		default: return "An unknown error occured during texture creating";
	}
}

void Texture::createTexture() {
	glGenTextures(1, &_imageData.texture);
	glBindTexture(GL_TEXTURE_2D, _imageData.texture);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB,
		_imageData.width,
		_imageData.height,
		0, _imageData.formatBMPImage, GL_UNSIGNED_BYTE,
		_imageData._pixelData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
	glActiveTexture(GL_TEXTURE0);
}

Texture::Texture(const std::string &path) :
	_bmpImage(BMPLoader(path.c_str()))
{
	_imageData.formatBMPImage =(_bmpImage.channels == 4) ? GL_BGRA : GL_BGR;
	_imageData.width = _bmpImage.getBMPHeader()->width;
	_imageData.height = _bmpImage.getBMPHeader()->height;
	_imageData._pixelData = _bmpImage.getPixelData();

	createTexture();
}

// getters

const ImageData &Texture::getImageData() const {
	return _imageData;
}

// other

void Texture::cleanUp() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
