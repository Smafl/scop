#pragma once

#include <exception>
#include <string>
#include <vector>

class BMPLoaderException : public std::exception {
public:
	enum ErrorCode {
		FILE_NOT_FOUND,
		CANNOT_OPEN,
		INVALID_FORMAT,
	};

	explicit BMPLoaderException(ErrorCode err, const std::string &path);
	const char *what() const noexcept override;

private:
	ErrorCode _errorCode;
	std::string _path;
	std::string _returnMessage;
};

struct Header {
	// Header 14 bytes
	uint16_t signature; // Must be 'BM': 0x4D42
	uint32_t fileSize; // size of file in bytes
	uint32_t reserved; // Must be 0
	uint32_t dataOffset; // Offset from beginning of file to the beginning of the bitmap data

	// InfoHeader 40 bytes
	uint32_t size; // Size of infoHeader, must be 40
	uint32_t widht; // Pixels
	uint32_t height; // Pixels
	uint16_t planes; // Must be 1
	uint16_t bitsPerPixel;
	uint32_t compression; // 0 if no compression
	uint32_t imageSize; // Can be 0 if compression is 0
	uint32_t horizontalResolution; // Pixels/meter
	uint32_t verticalResolution; // Pixels/meter
	uint32_t colorsUsed;
	uint32_t importantColors;
};

class BMPLoader {
public:
	int channels = 0;

	explicit BMPLoader(const std::string &path);

	const Header *getBMPHeader() const;
	const unsigned char *getPixelData() const;

private:
	std::string _path;
	Header _bmpHeader;
	std::vector<unsigned char> _pixelData; // BGR

	BMPLoader();
};
