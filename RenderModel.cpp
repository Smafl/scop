#include "RenderModel.hpp"
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/gl.h>

using namespace std;

RenderModelException::RenderModelException(ErrorCode err)
	: _errorCode(err) {}

const char *RenderModelException::what() const noexcept {
	switch (_errorCode) {
		case FILE_NOT_FOUND: return "File with rendering model not found";
		case CANNOT_OPEN: return "File with rendering model cannot be opened";
		case INVALID_FACE_FORMAT: return "Invalid face format in model file";
		case INDEX_OUT_OF_RANGE: return "Face index value is out of allowed range";
		case UNKNOWN_ERROR: return "Failed to parse a model file";
		default: return "An unknown error occurred during creating a render model";
	}
}

RenderModel::RenderModel(const string &path) :
	_path(path)
{
	if (_path.empty()) {
		throw RenderModelException(RenderModelException::FILE_NOT_FOUND);
	}

	ifstream file;
	file.open(_path);
	if (!file.is_open()) {
		throw RenderModelException(RenderModelException::CANNOT_OPEN);
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
			; // implement later
		} else if (type == "g") {
			; // to implement later
		} else if (type == "usemtl") {
			; // to implement later
		}
	}
	file.close();

	if (_vertices.empty() || _vIndices.empty()) {
		throw RenderModelException(RenderModelException::UNKNOWN_ERROR);
	}
}

const vector<GLfloat> &RenderModel::getVertices() const {
	return _vertices;
}

const vector<GLuint> &RenderModel::getTextures() const {
	return _texture;
}

const vector<GLuint> &RenderModel::getNormals() const {
	return _normals;
}

const vector<GLuint> &RenderModel::getVIndices() const {
	return _vIndices;
}

const vector<GLuint> &RenderModel::getVtIndices() const {
	return _vtIndices;
}

const vector<GLuint> &RenderModel::getVnIndices() const {
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

void RenderModel::parseFaces(istringstream &line) {
	vector<array<string, 3>> tokens;
	string token;

	while (line >> token) {
		tokens.push_back(getTokenValues(token));
	}

	int tokensSize = tokens.size();
	if (tokensSize < 3) {
		throw RenderModelException(RenderModelException::INVALID_FACE_FORMAT);
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
    	    throw RenderModelException(RenderModelException::INVALID_FACE_FORMAT);
    	} catch (const out_of_range&) {
    	    throw RenderModelException(RenderModelException::INDEX_OUT_OF_RANGE);
    	}
	}
}
