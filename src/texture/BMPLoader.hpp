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

#pragma pack(push, 1)
struct Header {
    uint16_t signature;      // 0x4D42
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;

    uint32_t size;           // info header size
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t horizontalResolution;
    uint32_t verticalResolution;
    uint32_t colorsUsed;
    uint32_t importantColors;
};
#pragma pack(pop)

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
