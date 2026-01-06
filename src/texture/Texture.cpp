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

Texture::Texture(const std::string &path) :
	_bmpImage(BMPLoader(path.c_str()))
{

	// if (!_bmpImage.getPixelData()) {
	// 	throw TextureException(TextureException::NO_DATA);
	// }

	_imageData.formatBMPImage =(_bmpImage.channels == 4) ? GL_BGRA : GL_BGR;
	_imageData.width = _bmpImage.getBMPHeader()->width;
	_imageData.height = _bmpImage.getBMPHeader()->height;
	_imageData._pixelData = _bmpImage.getPixelData();
}

// getters

const ImageData &Texture::getImageData() const {
	return _imageData;
}

// other

void Texture::cleanUp() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
