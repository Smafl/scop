#include "RenderModelLoader.hpp"
#include <exception>
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/gl.h>
#include <map>
#include <cfloat>  // FLT_MAX
#include <algorithm>  // min/max

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

	generateFinalVertices();
}

void RenderModelLoader::generateFinalVertices() {
    _finalVertices.clear();
    _finalIndices.clear();

    // Calculate bounding box for UV generation
    GLfloat minX = 1e10f, maxX = -1e10f;
    GLfloat minY = 1e10f, maxY = -1e10f;
    GLfloat minZ = 1e10f, maxZ = -1e10f;

    for (size_t i = 0; i < _vertices.size(); i += 4) {
        if (_vertices[i] < minX) minX = _vertices[i];
        if (_vertices[i] > maxX) maxX = _vertices[i];
        if (_vertices[i + 1] < minY) minY = _vertices[i + 1];
        if (_vertices[i + 1] > maxY) maxY = _vertices[i + 1];
        if (_vertices[i + 2] < minZ) minZ = _vertices[i + 2];
        if (_vertices[i + 2] > maxZ) maxZ = _vertices[i + 2];
    }

    GLfloat rangeX = maxX - minX;
    GLfloat rangeY = maxY - minY;
    GLfloat rangeZ = maxZ - minZ;

    if (rangeX < 0.0001f) rangeX = 1.0f;
    if (rangeY < 0.0001f) rangeY = 1.0f;
    if (rangeZ < 0.0001f) rangeZ = 1.0f;

    map<string, GLuint> uniqueVertices;
    bool hasTextureCoords = !_texture.empty() && !_vtIndices.empty();
    bool hasNormals = !_normals.empty() && !_vnIndices.empty();

    for (size_t i = 0; i < _vIndices.size(); i++) {
        GLuint vIdx = _vIndices[i];
        GLuint vtIdx = (i < _vtIndices.size()) ? _vtIndices[i] : 0;
        GLuint vnIdx = (i < _vnIndices.size()) ? _vnIndices[i] : 0;

        string key = to_string(vIdx) + "/" + to_string(vtIdx) + "/" + to_string(vnIdx);

        if (uniqueVertices.find(key) == uniqueVertices.end()) {
            GLuint newIndex = static_cast<GLuint>(_finalVertices.size() / 8);  // Now 8 floats per vertex!
            uniqueVertices[key] = newIndex;

            // Position (x, y, z)
            GLfloat posX = _vertices[vIdx * 4];
            GLfloat posY = _vertices[vIdx * 4 + 1];
            GLfloat posZ = _vertices[vIdx * 4 + 2];

            _finalVertices.push_back(posX);
            _finalVertices.push_back(posY);
            _finalVertices.push_back(posZ);

            // Texture coordinates (u, v)
            if (hasTextureCoords && vtIdx * 2 + 1 < _texture.size()) {
                _finalVertices.push_back(_texture[vtIdx * 2]);
                _finalVertices.push_back(_texture[vtIdx * 2 + 1]);
            } else {
                GLfloat u = (posX - minX) / rangeX;
                GLfloat v = (posY - minY) / rangeY;
                _finalVertices.push_back(u);
                _finalVertices.push_back(v);
            }

            // Color based on normal direction (to distinguish sides)
            GLfloat r, g, b;
            if (hasNormals && vnIdx * 3 + 2 < _normals.size()) {
                // Use normal direction to determine color
                GLfloat nx = _normals[vnIdx * 3];
                GLfloat ny = _normals[vnIdx * 3 + 1];
                GLfloat nz = _normals[vnIdx * 3 + 2];

                // Map normal direction to color
                // Faces pointing in different directions get different colors
                r = (nx + 1.0f) * 0.5f;  // Map -1..1 to 0..1
                g = (ny + 1.0f) * 0.5f;
                b = (nz + 1.0f) * 0.5f;
            } else {
                // Fallback: use position-based coloring
                r = (posX - minX) / rangeX;
                g = (posY - minY) / rangeY;
                b = (posZ - minZ) / rangeZ;
            }

            _finalVertices.push_back(r);
            _finalVertices.push_back(g);
            _finalVertices.push_back(b);
        }

        _finalIndices.push_back(uniqueVertices[key]);
    }

    if (!hasTextureCoords) {
        cout << "Note: Model has no UV coordinates, using auto-generated planar mapping" << endl;
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

const vector<GLfloat> &RenderModelLoader::getTextures() const {
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

	size_t maxVertexIndex = (_vertices.size() / 4);
	size_t maxTextureIndex = (_texture.size() / 2);
	size_t maxNormalIndex = (_normals.size() / 3);

	// how many triangles: tokensSize - 2
	for (int i = 1; i + 1 != tokensSize; i++) {
		try {
			// Vertex indices
			if ((!tokens[0][0].empty()) && (!tokens[i][0].empty()) && (!tokens[i + 1][0].empty())) {
				GLuint idx0 = stoul(tokens[0][0]) - 1;
				GLuint idx1 = stoul(tokens[i][0]) - 1;
				GLuint idx2 = stoul(tokens[i + 1][0]) - 1;

				// Validate indices
				if (idx0 >= maxVertexIndex || idx1 >= maxVertexIndex || idx2 >= maxVertexIndex) {
					throw RenderModelLoaderException(RenderModelLoaderException::INDEX_OUT_OF_RANGE);
				}

				_vIndices.push_back(idx0);
				_vIndices.push_back(idx1);
				_vIndices.push_back(idx2);
			}

			// Texture coordinate indices
			if ((!tokens[0][1].empty()) && (!tokens[i][1].empty()) && (!tokens[i + 1][1].empty())) {
				GLuint idx0 = stoul(tokens[0][1]) - 1;
				GLuint idx1 = stoul(tokens[i][1]) - 1;
				GLuint idx2 = stoul(tokens[i + 1][1]) - 1;

				// Validate indices
				if (idx0 >= maxTextureIndex || idx1 >= maxTextureIndex || idx2 >= maxTextureIndex) {
					throw RenderModelLoaderException(RenderModelLoaderException::INDEX_OUT_OF_RANGE);
				}

				_vtIndices.push_back(idx0);
				_vtIndices.push_back(idx1);
				_vtIndices.push_back(idx2);
			}

			// Normal indices
			if ((!tokens[0][2].empty()) && (!tokens[i][2].empty()) && (!tokens[i + 1][2].empty())) {
				GLuint idx0 = stoul(tokens[0][2]) - 1;
				GLuint idx1 = stoul(tokens[i][2]) - 1;
				GLuint idx2 = stoul(tokens[i + 1][2]) - 1;

				// Validate indices
				if (idx0 >= maxNormalIndex || idx1 >= maxNormalIndex || idx2 >= maxNormalIndex) {
					throw RenderModelLoaderException(RenderModelLoaderException::INDEX_OUT_OF_RANGE);
				}

				_vnIndices.push_back(idx0);
				_vnIndices.push_back(idx1);
				_vnIndices.push_back(idx2);
			}
    	} catch (const invalid_argument&) {
    	    throw RenderModelLoaderException(RenderModelLoaderException::INVALID_FACE_FORMAT);
    	} catch (const out_of_range&) {
    	    throw RenderModelLoaderException(RenderModelLoaderException::INDEX_OUT_OF_RANGE);
    	}
	}
}
