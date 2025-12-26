#include "RenderModelLoader.hpp"
#include <exception>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/gl.h>

using namespace std;

RenderModelLoaderException::RenderModelLoaderException(ErrorCode err)
	: _errorCode(err) {}

const char *RenderModelLoaderException::what() const noexcept {
	switch (_errorCode) {
		case FILE_NOT_FOUND: return "File with rendering model not found";
		case CANNOT_OPEN: return "File with rendering model cannot be opened";
		case INVALID_FACE_FORMAT: return "Invalid face format in model file";
		case INDEX_OUT_OF_RANGE: return "Face index value is out of allowed range";
		case UNKNOWN_ERROR: return "Failed to parse a model file";
		default: return "An unknown error occurred during creating a render model";
	}
}


/**
* @brief Loads and parses an OBJ file.
* @param path Path to the OBJ file.
* @throws RenderModelLoaderException on file or parsing errors.
*/
RenderModelLoader::RenderModelLoader(const string &path) :
	_path(path)
{
	if (_path.empty()) {
		throw RenderModelLoaderException(RenderModelLoaderException::FILE_NOT_FOUND);
	}

	ifstream file;
	file.open(_path);
	if (!file.is_open()) {
		throw RenderModelLoaderException(RenderModelLoaderException::CANNOT_OPEN);
	}

	string line, type;
	GLfloat x, y, z;
	while (getline(file, line)) {
		istringstream sline(line);
		sline >> type;
		if (type == "v") {
			sline >> x >> y >> z;
			_vertices.push_back(x);
			_vertices.push_back(y);
			_vertices.push_back(z);
			_vertices.push_back(1.0);
		} else if (type == "vt") {
			sline >> x >> y;
			_texture.push_back(x);
			_texture.push_back(y);
		} else if (type == "vn") {
			sline >> x >> y >> z;
			_normals.push_back(x);
			_normals.push_back(y);
			_normals.push_back(z);
		} else if (type == "f") {
			parseFaces(sline);
		} else if (type == "o") {
			; // to implement later
		} else if (type == "g") {
			; // to implement later
		} else if (type == "usemtl") {
			; // to implement later
		}
	}
	file.close();

	if (_vertices.empty() || _vIndices.empty()) {
		throw RenderModelLoaderException(RenderModelLoaderException::UNKNOWN_ERROR);
	}
}

// getters

const vector<GLfloat> &RenderModelLoader::getFinalVertices() const {
	return _finalVertices;
}

const vector<GLuint> &RenderModelLoader::getFinalIndices() const {
	return _finalIndices;
}

const vector<GLfloat> &RenderModelLoader::getVertices() const {
	return _vertices;
}

const vector<GLuint> &RenderModelLoader::getTextures() const {
	return _texture;
}

const vector<GLuint> &RenderModelLoader::getNormals() const {
	return _normals;
}

const vector<GLuint> &RenderModelLoader::getVIndices() const {
	return _vIndices;
}

const vector<GLuint> &RenderModelLoader::getVtIndices() const {
	return _vtIndices;
}

const vector<GLuint> &RenderModelLoader::getVnIndices() const {
	return _vnIndices;
}

// private //

array<string, 3> getTokenValues(string &token) {
	array<string, 3> tokenValues = {"", "", ""};

	size_t slash = token.find('/');
	if (slash == string::npos) {
		tokenValues[0] = token;
		return tokenValues;
	}

	size_t slash2 = token.find('/', slash + 1);
	tokenValues[0] = token.substr(0, slash);

	if (slash2 == string::npos) {
		tokenValues[1] = token.substr(slash + 1);
	} else {
		tokenValues[1] = token.substr(slash + 1, slash2 - slash - 1);
		tokenValues[2] = token.substr(slash2 + 1);
	}

	return tokenValues;
}

/**
* @brief Parses a face definition and generates triangle indices.
*
* Supports face formats:
*  - v
*  - v/vt
*  - v//vn
*  - v/vt/vn
*
* Polygons with more than three vertices are triangulated
* using a triangle fan method.
*
* @param line Input stream containing face tokens.
* @throws RenderModelLoaderException on invalid format or index errors.
*/
void RenderModelLoader::parseFaces(istringstream &line) {
	vector<array<string, 3>> tokens;
	string token;

	while (line >> token) {
		tokens.push_back(getTokenValues(token));
	}

	int tokensSize = tokens.size();
	if (tokensSize < 3) {
		throw RenderModelLoaderException(RenderModelLoaderException::INVALID_FACE_FORMAT);
	}

	// how many triangles: tokensSize - 2
	for (int i = 1; i + 1 != tokensSize; i++) {
		try {
			if ((!tokens[0][0].empty()) && (!tokens[i][0].empty()) && (!tokens[i + 1][0].empty())) {
				_vIndices.push_back(stoul(tokens[0][0]) - 1);
				_vIndices.push_back(stoul(tokens[i][0]) - 1);
				_vIndices.push_back(stoul(tokens[i + 1][0]) - 1);
			}

			if ((!tokens[0][1].empty()) && (!tokens[i][1].empty()) && (!tokens[i + 1][1].empty())) {
				_vtIndices.push_back(stoul(tokens[0][1]) - 1);
				_vtIndices.push_back(stoul(tokens[i][1]) - 1);
				_vtIndices.push_back(stoul(tokens[i + 1][1]) - 1);
			}

			if ((!tokens[0][2].empty()) && (!tokens[i][2].empty()) && (!tokens[i + 1][2].empty())) {
				_vnIndices.push_back(stoul(tokens[0][2]) - 1);
				_vnIndices.push_back(stoul(tokens[i][2]) - 1);
				_vnIndices.push_back(stoul(tokens[i + 1][2]) - 1);
			}
    	} catch (const invalid_argument&) {
    	    throw RenderModelLoaderException(RenderModelLoaderException::INVALID_FACE_FORMAT);
    	} catch (const out_of_range&) {
    	    throw RenderModelLoaderException(RenderModelLoaderException::INDEX_OUT_OF_RANGE);
    	}
	}
}
