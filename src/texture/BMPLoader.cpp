#include "BMPLoader.hpp"
#include <exception>
#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

BMPLoaderException::BMPLoaderException(ErrorCode err, const string &path)
	: _errorCode(err), _path(path)
{
	if (_path.empty()) {
		_path = "no path given";
	}

	switch(_errorCode) {
		case FILE_NOT_FOUND:
			_returnMessage = "File with rendering model not found: " + _path;
			break;
		case CANNOT_OPEN:
			_returnMessage = "File with rendering model cannot be opened: " + _path;
			break;
		case INVALID_FORMAT:
			_returnMessage = "Invalid file format: " + _path;
			break;
		default:
			_returnMessage = "An unknown error occurred during BMP image loading: " + _path;
			break;
	}
}

const char *BMPLoaderException::what() const noexcept {
	return _returnMessage.c_str();
}

BMPLoader::BMPLoader(const string &path)
    : _path(path)
{
    cout << "BMP is starting loading from " << _path << endl;
    if (_path.empty()) {
        throw BMPLoaderException(BMPLoaderException::FILE_NOT_FOUND, _path);
    }

    _path = "../" + path;
    FILE* file = fopen(_path.c_str(), "rb");
    if (!file) {
        throw BMPLoaderException(BMPLoaderException::CANNOT_OPEN, _path);
    }

    fread(&_bmpHeader, sizeof(Header), 1, file);
    if (_bmpHeader.signature != 0x4D42) {
        fclose(file);
        throw BMPLoaderException(BMPLoaderException::INVALID_FORMAT, _path);
    }

    channels = _bmpHeader.bitsPerPixel / 8;
    if (channels != 3 && channels != 4) {
        fclose(file);
        throw BMPLoaderException(BMPLoaderException::INVALID_FORMAT, _path);
    }

    // Calculate row size WITH padding (rows are padded to 4-byte boundary)
    int rowSizePadded = ((_bmpHeader.width * _bmpHeader.bitsPerPixel + 31) / 32) * 4;
    // Calculate row size WITHOUT padding
    int rowSize = _bmpHeader.width * channels;

    // Read the padded data from file
    vector<unsigned char> paddedData(rowSizePadded * _bmpHeader.height);
    fseek(file, _bmpHeader.dataOffset, SEEK_SET);
    fread(paddedData.data(), 1, paddedData.size(), file);
    fclose(file);

    // Allocate final buffer without padding, and flip vertically
    _pixelData.resize(_bmpHeader.width * _bmpHeader.height * channels);

    unsigned char* dstRow;
    // Copy row by row, removing padding and flipping
    for (uint32_t y = 0; y < _bmpHeader.height; ++y) {
        dstRow = &_pixelData[y * rowSize];
        // Read from bottom to top (flip vertically)
        unsigned char* srcRow = &paddedData[(_bmpHeader.height - 1 - y) * rowSizePadded];
        memcpy(dstRow, srcRow, rowSize);
    }

    // for (size_t i = 0; i < _pixelData.size(); i += channels) {
    //     std::swap(_pixelData[i], _pixelData[i + 2]); // Swap B and R channels
    // }

    // cout << "BMP loaded: " << _bmpHeader.width << "x" << _bmpHeader.height
    //      << ", " << channels << " channels" << endl;
}

const Header *BMPLoader::getBMPHeader() const {
	return &_bmpHeader;
}

const unsigned char *BMPLoader::getPixelData() const {
	return _pixelData.data();
}
