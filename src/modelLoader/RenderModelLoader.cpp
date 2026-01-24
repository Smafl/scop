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

	parseOBJFile();
	calculateNormals();

	if (_raw.vertices.empty() || _raw.vIndices.empty()) {
		throw RenderModelLoaderException(RenderModelLoaderException::UNKNOWN_ERROR);
	}

	buildMesh();
}

void RenderModelLoader::parseOBJFile() {
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
			_raw.vertices.push_back(x);
			_raw.vertices.push_back(y);
			_raw.vertices.push_back(z);
			_raw.vertices.push_back(1.0);
		} else if (type == "vt") {
			sline >> x >> y;
			_raw.texCoords.push_back(x);
			_raw.texCoords.push_back(y);
		} else if (type == "vn") {
			sline >> x >> y >> z;
			_raw.normals.push_back(x);
			_raw.normals.push_back(y);
			_raw.normals.push_back(z);
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
}

void RenderModelLoader::calculateBoundingBox() {
	for (size_t i = 0; i < _raw.vertices.size(); i += 4) {
		_bbox.minX = min(_bbox.minX, _raw.vertices[i]);
		_bbox.maxX = max(_bbox.maxX, _raw.vertices[i]);
		_bbox.minY = min(_bbox.minY, _raw.vertices[i + 1]);
		_bbox.maxY = max(_bbox.maxY, _raw.vertices[i + 1]);
		_bbox.minZ = min(_bbox.minZ, _raw.vertices[i + 2]);
		_bbox.maxZ = max(_bbox.maxZ, _raw.vertices[i + 2]);
	}
}

void RenderModelLoader::calculateCubicUV() {
	GLfloat u, v;

    // Find which axis the normal is most aligned with
    GLfloat absNX = fabs(_nx);
    GLfloat absNY = fabs(_ny);
    GLfloat absNZ = fabs(_nz);

    if (absNX >= absNY && absNX >= absNZ) {
        // X-dominant face - project from YZ plane
        u = (_posZ - _bbox.minZ) / _bbox.getRangeZ();
        v = (_posY - _bbox.minY) / _bbox.getRangeY();
    } else if (absNY >= absNX && absNY >= absNZ) {
        // Y-dominant face - project from XZ plane
        u = (_posX - _bbox.minX) / _bbox.getRangeX();
        v = (_posZ - _bbox.minZ) / _bbox.getRangeZ();
    } else {
        // Z-dominant face - project from XY plane
        u = (_posX - _bbox.minX) / _bbox.getRangeX();
        v = (_posY - _bbox.minY) / _bbox.getRangeY();
    }

	_mesh.vertices.push_back(u);
	_mesh.vertices.push_back(v);
}

void RenderModelLoader::calculateUVCoordinates(GLuint vtIdx, GLuint vnIdx) {
	if (_raw.hasTexCoords() && vtIdx * 2 + 1 < _raw.texCoords.size()) {
		_mesh.vertices.push_back(_raw.texCoords[vtIdx * 2]);
		_mesh.vertices.push_back(_raw.texCoords[vtIdx * 2 + 1]);
	} else {
		if (_raw.hasNormals() && vnIdx * 3 + 2 < _raw.normals.size()) {
			// Use cubic/box mapping based on normal direction
			_nx = _raw.normals[vnIdx * 3];
            _ny = _raw.normals[vnIdx * 3 + 1];
            _nz = _raw.normals[vnIdx * 3 + 2];

			calculateCubicUV();
		} else {
			// Fallback to simple planar mapping
			GLfloat u = (_posX - _bbox.minX) / _bbox.getRangeX();
			GLfloat v = (_posY - _bbox.minY) / _bbox.getRangeY();
			_mesh.vertices.push_back(u);
			_mesh.vertices.push_back(v);
		}
	}
}

void RenderModelLoader::calculateVertexColor(GLuint vnIdx) {
	// Color based on normal direction (to distinguish sides)
	GLfloat r, g, b;
	if (_raw.hasNormals() && vnIdx * 3 + 2 < _raw.normals.size()) {
		// Use normal direction to determine color
		GLfloat nx = _raw.normals[vnIdx * 3];
		GLfloat ny = _raw.normals[vnIdx * 3 + 1];
		GLfloat nz = _raw.normals[vnIdx * 3 + 2];

		// Map normal direction to color
		// Faces pointing in different directions get different colors
		r = (nx + 1.0f) * 0.5f;  // Map -1..1 to 0..1
		g = (ny + 1.0f) * 0.5f;
		b = (nz + 1.0f) * 0.5f;
	} else {
		// Fallback: use position-based coloring
		r = (_posX - _bbox.minX) / _bbox.getRangeX();
		g = (_posY - _bbox.minY) / _bbox.getRangeY();
		b = (_posZ - _bbox.minZ) / _bbox.getRangeZ();
	}

	_mesh.vertices.push_back(r);
	_mesh.vertices.push_back(g);
	_mesh.vertices.push_back(b);
}

void RenderModelLoader::calculateNormals() {
    if (!_raw.normals.empty()) {
        return;
    }

    std::cout << "No normals found, calculating from geometry...\n";

    // Reserve space for per-vertex normals (initialized to zero)
    _raw.normals.resize(_raw.vertices.size() / 4 * 3, 0.0f);
    _raw.vnIndices.resize(_raw.vIndices.size());

    // Calculate face normals and accumulate at vertices
    for (size_t i = 0; i < _raw.vIndices.size(); i += 3) {
        GLuint idx0 = _raw.vIndices[i];
        GLuint idx1 = _raw.vIndices[i + 1];
        GLuint idx2 = _raw.vIndices[i + 2];

        // Get triangle vertices
        GLfloat v0x = _raw.vertices[idx0 * 4];
        GLfloat v0y = _raw.vertices[idx0 * 4 + 1];
        GLfloat v0z = _raw.vertices[idx0 * 4 + 2];

        GLfloat v1x = _raw.vertices[idx1 * 4];
        GLfloat v1y = _raw.vertices[idx1 * 4 + 1];
        GLfloat v1z = _raw.vertices[idx1 * 4 + 2];

        GLfloat v2x = _raw.vertices[idx2 * 4];
        GLfloat v2y = _raw.vertices[idx2 * 4 + 1];
        GLfloat v2z = _raw.vertices[idx2 * 4 + 2];

        // Calculate edge vectors
        GLfloat e1x = v1x - v0x;
        GLfloat e1y = v1y - v0y;
        GLfloat e1z = v1z - v0z;

        GLfloat e2x = v2x - v0x;
        GLfloat e2y = v2y - v0y;
        GLfloat e2z = v2z - v0z;

        // Cross product: normal = e1 × e2
        GLfloat nx = e1y * e2z - e1z * e2y;
        GLfloat ny = e1z * e2x - e1x * e2z;
        GLfloat nz = e1x * e2y - e1y * e2x;

        // Normalize
        GLfloat len = sqrt(nx*nx + ny*ny + nz*nz);
        if (len > 0.0001f) {
            nx /= len;
            ny /= len;
            nz /= len;
        }

        // Accumulate normal at each vertex
        _raw.normals[idx0 * 3]     += nx;
        _raw.normals[idx0 * 3 + 1] += ny;
        _raw.normals[idx0 * 3 + 2] += nz;

        _raw.normals[idx1 * 3]     += nx;
        _raw.normals[idx1 * 3 + 1] += ny;
        _raw.normals[idx1 * 3 + 2] += nz;

        _raw.normals[idx2 * 3]     += nx;
        _raw.normals[idx2 * 3 + 1] += ny;
        _raw.normals[idx2 * 3 + 2] += nz;

        // Set normal indices (same as vertex indices)
        _raw.vnIndices[i]     = idx0;
        _raw.vnIndices[i + 1] = idx1;
        _raw.vnIndices[i + 2] = idx2;
    }

    // Normalize all accumulated normals
    for (size_t i = 0; i < _raw.normals.size(); i += 3) {
        GLfloat nx = _raw.normals[i];
        GLfloat ny = _raw.normals[i + 1];
        GLfloat nz = _raw.normals[i + 2];

        GLfloat len = sqrt(nx*nx + ny*ny + nz*nz);
        if (len > 0.0001f) {
            _raw.normals[i]     /= len;
            _raw.normals[i + 1] /= len;
            _raw.normals[i + 2] /= len;
        }
	}
}

void RenderModelLoader::buildMesh() {
	// DEBUG: Check if we have normals
    std::cout << "\n=== Mesh Build Debug Info ===\n";
    std::cout << "Has normals: " << (_raw.hasNormals() ? "YES" : "NO") << "\n";
    std::cout << "Has texCoords: " << (_raw.hasTexCoords() ? "YES" : "NO") << "\n";
    std::cout << "Total vertices: " << (_raw.vertices.size() / 4) << "\n";
    std::cout << "Total normals: " << (_raw.normals.size() / 3) << "\n";
    std::cout << "Total faces (indices): " << (_raw.vIndices.size() / 3) << "\n\n";

    _mesh.vertices.clear();
    _mesh.indices.clear();

	calculateBoundingBox();

    map<string, GLuint> uniqueVertices;

    for (size_t i = 0; i < _raw.vIndices.size(); i++) {
        GLuint vIdx = _raw.vIndices[i];
        GLuint vtIdx = (i < _raw.vtIndices.size()) ? _raw.vtIndices[i] : 0;
        GLuint vnIdx = (i < _raw.vnIndices.size()) ? _raw.vnIndices[i] : 0;

        string key = to_string(vIdx) + "/" + to_string(vtIdx) + "/" + to_string(vnIdx);

        if (uniqueVertices.find(key) == uniqueVertices.end()) {
            GLuint newIndex = static_cast<GLuint>(_mesh.vertices.size() / 8);
            uniqueVertices[key] = newIndex;

            // Position (x, y, z)
            _posX = _raw.vertices[vIdx * 4];
            _posY = _raw.vertices[vIdx * 4 + 1];
            _posZ = _raw.vertices[vIdx * 4 + 2];

            _mesh.vertices.push_back(_posX);
            _mesh.vertices.push_back(_posY);
            _mesh.vertices.push_back(_posZ);

			calculateUVCoordinates(vtIdx, vnIdx);
			calculateVertexColor(vnIdx);
        }

        _mesh.indices.push_back(uniqueVertices[key]);
    }

    std::cout << "\nFinal mesh vertices: " << (_mesh.vertices.size() / 8) << "\n";
    std::cout << "Final mesh indices: " << _mesh.indices.size() << "\n";
    std::cout << "===========================\n\n";
}

// getters

const vector<GLfloat> &RenderModelLoader::getFinalVertices() const {
	return _mesh.vertices;
}

const vector<GLuint> &RenderModelLoader::getFinalIndices() const {
	return _mesh.indices;
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

	size_t maxVertexIndex = (_raw.vertices.size() / 4);
	size_t maxTextureIndex = (_raw.texCoords.size() / 2);
	size_t maxNormalIndex = (_raw.normals.size() / 3);

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

				_raw.vIndices.push_back(idx0);
				_raw.vIndices.push_back(idx1);
				_raw.vIndices.push_back(idx2);
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

				_raw.vtIndices.push_back(idx0);
				_raw.vtIndices.push_back(idx1);
				_raw.vtIndices.push_back(idx2);
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

				_raw.vnIndices.push_back(idx0);
				_raw.vnIndices.push_back(idx1);
				_raw.vnIndices.push_back(idx2);
			}
    	} catch (const invalid_argument&) {
    	    throw RenderModelLoaderException(RenderModelLoaderException::INVALID_FACE_FORMAT);
    	} catch (const out_of_range&) {
    	    throw RenderModelLoaderException(RenderModelLoaderException::INDEX_OUT_OF_RANGE);
    	}
	}
}
