#include "RenderModelLoader.hpp"
#include <exception>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <initializer_list>
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
	while (getline(file, line)) {
		istringstream sline(line);
		sline >> type;
		if (type == "v") {
			Vertex vertex;
			sline >> vertex.x >> vertex.y >> vertex.z;
			_vertex.push_back(vertex);
		} else if (type == "vt") {
			TexCoord textCoord;
			sline >> textCoord.u >> textCoord.v;
			_texcoord.push_back(textCoord);
		} else if (type == "vn") {
			Normal normal;
			sline >> normal.x >> normal.y >> normal.z;
			_normal.push_back(normal);
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

	buildMesh();

	std::cout << "Mesh stats" << endl;
	std::cout << "Vertices: " << _mesh.vertices.size() / 3 << endl;
	std::cout << "TexCoords: " << _mesh.texCoords.size() / 2 << endl << endl;

	std::cout << "Vertices coords" << endl;
	for (size_t i = 0; i < _mesh.vertices.size(); i += 3) {
	    std::cout << "  Vertices: " << _mesh.vertices[i] << ", " << _mesh.vertices[i+1] << ", " << _mesh.vertices[i+2] << endl;
	}

	std::cout << "UV coords" << endl;
	for (size_t i = 0; i < _mesh.texCoords.size(); i += 2) {
	    std::cout << "  UV: " << _mesh.texCoords[i] << ", " << _mesh.texCoords[i+1] << endl;
	}

	std::cout << "Normals coords" << endl;
	for (size_t i = 0; i < _mesh.normals.size(); i += 3) {
	    std::cout << "  Normals: " << _mesh.normals[i] << ", " << _mesh.normals[i+1] << ", " << _mesh.normals[i+2] << endl;
	}

	std::cout << "Indices coords" << endl;
	for (auto i : _mesh.indices) {
		cout << i << " ";
	}
	cout << endl;
}

void RenderModelLoader::buildMesh() {
	GLuint currentIndex = 0;

	// Random number generator for colors
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.3f, 1.0f);

	for (const auto &face : _faces) {

		float r = dis(gen);
        float g = dis(gen);
        float b = dis(gen);

		for (size_t i = 1; i < face.size() -1; i++) {
			for (size_t j : initializer_list<size_t>{0, i, i + 1}) {
				const FaceVertex &fv = face[j]
;				// Add vertex
                if (fv.vertexIndex >= 0 && (size_t)fv.vertexIndex < _vertex.size()) {
                    const Vertex& v = _vertex[fv.vertexIndex];
                    _mesh.vertices.push_back(v.x);
                    _mesh.vertices.push_back(v.y);
                    _mesh.vertices.push_back(v.z);
                }
                // Add texture coordinate
                if (fv.texCoordIndex >= 0 && (size_t)fv.texCoordIndex < _texcoord.size()) {
                    const TexCoord& vt = _texcoord[fv.texCoordIndex];
                    _mesh.texCoords.push_back(vt.u);
                    _mesh.texCoords.push_back(vt.v);
                } else {
                    _mesh.texCoords.push_back(0.0f);
                    _mesh.texCoords.push_back(0.0f);
                }
                // Add normal
                if (fv.normalIndex >= 0 && (size_t)fv.normalIndex < _normal.size()) {
                    const Normal& vn = _normal[fv.normalIndex];
                    _mesh.normals.push_back(vn.x);
                    _mesh.normals.push_back(vn.y);
                    _mesh.normals.push_back(vn.z);
                } else {
                    _mesh.normals.push_back(0.0f);
                    _mesh.normals.push_back(0.0f);
                    _mesh.normals.push_back(1.0f);
                }

                _mesh.colors.push_back(r);
                _mesh.colors.push_back(g);
                _mesh.colors.push_back(b);

				_mesh.indices.push_back(currentIndex++);
			}
		}
	}
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
	vector<FaceVertex> face;
	string token;

	while (line >> token) {
		FaceVertex fv = {-1, -1, -1};

		size_t pos1 = token.find('/');
		if (pos1 == string::npos) {
			// Format v
			fv.vertexIndex = stoi(token) - 1;
		} else {
			fv.vertexIndex = stoi(token.substr(0, pos1)) - 1;

			size_t pos2 = token.find('/', pos1 + 1);
			if (pos2 == string::npos) {
				// Format v/vt
				fv.texCoordIndex = stoi(token.substr(pos1 + 1)) - 1;
			} else {
				// Format v/vt/vn or v//vn
				if (pos2 > pos1 + 1) {
					fv.texCoordIndex = stoi(token.substr(pos1 + 1, pos2 - pos1 - 1)) - 1;
				}
				fv.normalIndex = stoi(token.substr(pos2 + 1)) - 1;
			}
		}
		face.push_back(fv);
	}
	_faces.push_back(face);
}

const Mesh &RenderModelLoader::getMesh() const {
	return _mesh;
}
